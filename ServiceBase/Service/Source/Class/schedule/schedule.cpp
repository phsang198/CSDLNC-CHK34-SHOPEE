#pragma once
#include "pch.h"
#include "schedule.h"

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <unordered_map>

#include <chrono>
#include "Model/ScheduleModel/ScheduleModel.h"
#include "Validate/Validate.h"

using mapschedule = std::map<std::string, std::unordered_map<std::string, std::shared_ptr<schedule::registration>>>;

std::string schedule::wserver::host = "10.222.3.84";
std::string schedule::wserver::WFSVPort = "18085";

std::map<std::string, std::unordered_map<std::string, std::shared_ptr<schedule::registration>>> schedule::manager::m_schedule;
std::unordered_map<std::string, std::shared_ptr<schedule::registration>> schedule::manager::m_excuteSchedule;

std::mutex smt;

schedule::Timer schedule::manager::timer;
//------------------------------------------------------------------------------------------------
schedule::registration::registration()
{

}
schedule::registration::registration(std::string _scdname, std::string _wid,
                                     int _itimeout, int _wtimeout, std::string _inss, std::string _id)
{
    scdname = _scdname; 
    wid = _wid;
    itimeout = _itimeout; 
    wtimeout = _wtimeout;
    inss = _inss;
    scdid = _id; 
    parse_ins(); 
}
schedule::registration::registration(std::string _url, std::string _method, std::string _body, std::string _authorization, ToEpochTime _timepoint, std::string _id)
{
    url = _url; 
    method = _method;
    timepoint = _timepoint;
    body = _body;
    authorization = _authorization;
    scdid = _id;
}
schedule::registration::~registration()
{
}

void schedule::registration::parse_ins()
{
    li.clear();

    json body = json::parse(inss);

    for (auto& inst : body)
    {
        std::string input = inst[RQ_SCHEDULE_PARAM_INPUT].dump();
        std::string iid = inst[RQ_SCHEDULE_PARAM_INSTANCEID];
        std::string status = inst[RQ_SCHEDULE_PARAM_STATUS];
        int starttime = inst[RQ_SCHEDULE_PARAM_STARTTIME];

        auto is = std::make_shared<schedule::sinstance>(iid, input, starttime);
        is->status = status;
        is->itimeout = itimeout;

        li.push_back(std::move(is));
    }
}
void schedule::registration::li2json()
{
    json tmp = json::array();

    for (auto& is : li)
    {
        json obj;
        obj[RQ_SCHEDULE_PARAM_INPUT] = json::parse(is->input);
        obj[RQ_SCHEDULE_PARAM_INSTANCEID] = is->iid;
        obj[RQ_SCHEDULE_PARAM_STATUS] = is->status;
        obj[RQ_SCHEDULE_PARAM_STARTTIME] = is->starttime;

        tmp.push_back(obj);
    }
    inss = tmp.dump();
}
//------------------------------------------------------------------------------------------------
void schedule::manager::add(std::shared_ptr<schedule::registration> reg)
{
    std::lock_guard<std::mutex> lk(smt);

    schedule::manager::m_schedule[reg->wid][reg->scdid] = reg;

    for (auto _ins : reg->li)
    {
        if (_ins->status == "Open")
        {
            auto fnc = std::bind([&](std::shared_ptr<schedule::registration>& reg, std::shared_ptr<sinstance>& ins)
                {
                    reg->runTask(ins.get());
                }, reg,_ins);

            schedule::manager::timer.addTask(fnc, _ins->starttime);

        }
    }
}
void schedule::manager::deleteIs(std::string scdid, std::string iid)
{
    std::lock_guard<std::mutex> lk(smt);
    //if (scdid == "") m_schedule.clear(); 
    //else
    //{
    //    for (auto& x : m_schedule)
    //    {
    //        /* for (auto & y : x.second)
    //             if (y.scdid == scdid)
    //             {
    //                 int pos = -1;
    //                 for (int i = 0; i < y.li.size(); i++)
    //                     if (y.li[i].iid == iid)
    //                     {
    //                         pos = i;
    //                         break;
    //                     }
    //                 if (pos != -1) y.li.erase(y.li.begin() +pos);
    //             }*/
    //        int pos = -1;
    //        for (int i = 0; i < x.second.size(); i++)
    //            if (x.second[i].scdid == scdid)
    //            {
    //                pos = i;
    //                break;
    //            }
    //        if (pos != -1) x.second.erase(x.second.begin() + pos);
    //    }
    //}
}

std::string schedule::manager::getstatus(std::string scdid)
{
    for ( auto w : m_schedule)
        if (w.second.find(scdid) != w.second.end())
        {
            for (auto i : w.second.at(scdid)->li)
                return i->status; 
        }
    return "Fail"; 
}

std::shared_ptr<schedule::registration> schedule::manager::updateIStatus(std::string wid, std::string iid, std::string status, std::string scdid)
{
    std::lock_guard<std::mutex> lk(smt);
    if (scdid != "")
    {
        for (auto& ins : m_schedule[wid][scdid]->li)
            if (ins->iid == iid && ins->status == "Started")
            {
                ins->status = status;
                m_schedule[wid][scdid]->li2json();
                return m_schedule[wid][scdid];
            }
    }
    else
    {
        for (auto& reg : m_schedule[wid])
        {
            for (auto& ins : reg.second->li)
                if (ins->iid == iid && ins->status == "Started")
                {
                    ins->status = status;
                    reg.second->li2json();
                    return reg.second;
                }
        }
    }
    return std::shared_ptr<schedule::registration>(); 
}
//------------------------------------------------------------------------------------------------
schedule::sinstance::sinstance(std::string _iid, std::string _input, int64_t _starttime)
{
    iid = _iid;
    input = _input;
    starttime = _starttime;
    auto now = std::chrono::system_clock::now();
    int64_t tmp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() / 1000;
    if (tmp >= starttime)
        status = "Overtime";
    else status = "Open";
}
void schedule::registration::runTask(schedule::sinstance* ins)
{
    std::string url = "http://$host:$WFSVPort/workflow/v1/instances?func=start&id=$id";
    StringProcess::Replace(url, "$host", schedule::wserver::host);
    StringProcess::Replace(url, "$WFSVPort", schedule::wserver::WFSVPort);
    StringProcess::Replace(url, "$id", ins->iid);

    cpr::Response r = cpr::Patch(cpr::Url{ url }, cpr::Timeout(1500), cpr::ConnectTimeout(1500),
        cpr::Header{ {"token",ins->token} }, cpr::Body(ins->input));

    Poco::Logger::root().information(url, __FILE__, __LINE__);

    ins->status = "Started";

    std::string _stt = "";

    if (r.status_code == 200)
    {
        json res = json::parse(r.text);
        if (res["code"] != 200)
        {
            _stt = "Failed";
        }
        if (res.find("detail") != res.end())
        {
            std::string tmp = res["detail"];
            Poco::Logger::root().information(tmp, __FILE__, __LINE__);
        }
    }
    else
    {
        _stt = "Failed";
    }
    if (_stt == "Failed")
    {
        auto reg = schedule::manager::updateIStatus(wid, ins->iid, _stt, scdid);
        ScheduleModel::updateSchedule(reg, 1);
    }
}
//------------------------------------------------------------------------------------------------
using rBody = validate::body;
#define bGetVal			rBody::getValue 

std::shared_ptr<schedule::registration> schedule::registration::parserExcuteSchedule(std::string _body)
{
    crow::response res;
    auto x = crow::json::load(_body);

    std::string url = bGetVal(3, res, x, "url");
    std::string method = bGetVal(3, res, x, "method");
    std::string body = bGetVal(4, res, x, "body");
    std::string Trigger_Interval = bGetVal(3, res, x, "Trigger_Interval");
    std::string name = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_SCHEDULENAME);
    std::string token = bGetVal(3, res, x, "token");

    if (Trigger_Interval == "") return nullptr;
    json tmp_p = json::parse(_body);
    if (tmp_p.count("body") != 0)
        body = tmp_p["body"].dump();

    ToEpochTime tept;
    tept.Trigger_Interval = Trigger_Interval;
    if (Trigger_Interval == "Seconds")
    {
        std::string sbt = bGetVal(2, res, x, "Seconds_between_Triggers");
        tept.sTrigger = ToEpochTime::STrigger({ std::stoi(sbt) });
    }
    else if (Trigger_Interval == "Minutes")
    {
        std::string mbt = bGetVal(2, res, x, "Minutes_between_Triggers");
        tept.mTrigger = ToEpochTime::MTrigger({ std::stoi(mbt) });
    }
    else if (Trigger_Interval == "Hours")
    {
        std::string hbt = bGetVal(2, res, x, "Hours_between_Triggers");
        std::string tam = bGetVal(2, res, x, "Trigger_at_Minute");
        tept.hTrigger = ToEpochTime::HTrigger({ std::stoi(hbt),std::stoi(tam) });
    }
    else if (Trigger_Interval == "Days")
    {
        std::string dbt = bGetVal(2, res, x, "Days_between_Triggers");
        std::string tah = bGetVal(2, res, x, "Trigger_at_Hour");
        std::string tam = bGetVal(2, res, x, "Trigger_at_Minute");
        tept.dTrigger = ToEpochTime::DTrigger({ std::stoi(dbt),std::stoi(tah),std::stoi(tam) });
    }
    else if (Trigger_Interval == "Timepoint")
    {
        tept.startTime = bGetVal(3, res, x, "Start_Time");
    }
    else
    {
        return nullptr; 
    }

    std::shared_ptr<schedule::registration> reg = std::make_shared<schedule::registration>
        (url, method, body, token, tept);

    return reg;
}
//------------------------------------------------------------------------------------------------
void schedule::manager::initFrequent(std::shared_ptr<schedule::registration> reg)
{
    std::lock_guard<std::mutex> lk(smt);
    m_excuteSchedule[reg->scdid] = reg;
    reg->state = schedule::registration::Starting;
    if (reg->timepoint.startTime != "")
    {
        auto tmp = std::chrono::system_clock::now();
        int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(tmp.time_since_epoch()).count() / 1000;
        if ( now <= ToEpochTime::convertToEpoch(reg->timepoint.startTime))
            addTimePoint(reg);
        else 
            reg->state = schedule::registration::Expired;
    }
    else 
        addFrequent(reg);
}

void schedule::manager::addTimePoint(std::shared_ptr<schedule::registration> reg)
{
    Poco::Logger::root().information("status id " + reg->scdid + " is " + reg->getState(), __FILE__, __LINE__);
    //Poco::Logger::root().information("timepoint D id " + reg->scdid + " is " + std::to_string(reg->timepoint.dTrigger.Days_between_Triggers), __FILE__, __LINE__);

    auto fnc = std::bind([](std::shared_ptr<schedule::registration> reg)
        {
            if (!reg) return;
            if (!(reg->state == schedule::registration::Running || reg->state == schedule::registration::Starting))
            {
                Poco::Logger::root().information("id " + reg->scdid + " not running", __FILE__, __LINE__);
                return;
            }
            reg->excute();
            reg->state = schedule::registration::Stopped;
        }, reg);

    schedule::manager::timer.addTask(fnc, ToEpochTime::convertToEpoch(reg->timepoint.startTime));
}

void schedule::manager::addFrequent(std::shared_ptr<schedule::registration> reg)
{
    Poco::Logger::root().information("status id " + reg->scdid + " is " + reg->getState(), __FILE__, __LINE__);
    //Poco::Logger::root().information("timepoint D id " + reg->scdid + " is " + std::to_string(reg->timepoint.dTrigger.Days_between_Triggers), __FILE__, __LINE__);

    auto fnc = std::bind([](std::shared_ptr<schedule::registration> reg)
        {
            if (!reg) return; 
            if (!(reg->state == schedule::registration::Running || reg->state == schedule::registration::Starting ))
            {
                Poco::Logger::root().information("id " + reg->scdid + " not running", __FILE__, __LINE__);
                return;
            }
            reg->state = schedule::registration::Running; 
            schedule::manager::addFrequent(reg);
            reg->excute();
        }, reg);

    schedule::manager::timer.addTask(fnc, reg->timepoint.get(reg->state == schedule::registration::Starting ? 1:0));
}

std::shared_ptr<schedule::registration> schedule::manager::getFrequent(std::string_view scdid)
{
    std::lock_guard<std::mutex> lk(smt);
    if (m_excuteSchedule.count(scdid.data()))
    {
        return m_excuteSchedule[scdid.data()];
    }
    return nullptr;
}

int schedule::manager::restartFrequent(std::string scdid)
{
    auto req = getFrequent(scdid);
    if (req)
    {
        if (req->state == schedule::registration::Running) return 200;
        req->state = schedule::registration::Running;
        schedule::manager::addFrequent(m_excuteSchedule[scdid]);
        return 200;
    }
    return 303;
}

int schedule::manager::stopFrequent(std::string scdid)
{
    auto req = getFrequent(scdid);
    if (req)
    {
        req->state = schedule::registration::Stopping;
        return 200;
    }
    return 303;
}

int schedule::manager::tryFunc(std::string scdid)
{
    auto req = getFrequent(scdid);
    if (req)
    {
        req->tryFunc();
        return 200;
    }
    return 303;
}
//------------------------------------------------------------------------------------------------
int schedule::manager::deleteExcute(std::string scdid)
{
    if (scdid == "") 
    {
        std::lock_guard<std::mutex> lk(smt);
        m_excuteSchedule.clear();
        return 200;
    };

    auto req = getFrequent(scdid);
    if (req)
    {
        std::lock_guard<std::mutex> lk(smt);
        req->state = schedule::registration::Deleted;
        m_excuteSchedule.erase(scdid);

        return 200;
    }
    return 303; 
}
//------------------------------------------------------------------------------------------------
#define param cpr::Url{ url }, cpr::Header{ {"token",authorization},{"Content-Type","application/json"} }, cpr::Timeout(1500), cpr::ConnectTimeout(1500), cpr::Body(body)
void schedule::registration::excute()
{
    //std::shared_ptr<schedule::registration> reg(url, method, body, authorization, timepoint,scdid);

    //schedule::manager::addFrequent(this,0);
    state = schedule::registration::Running;

    std::string log = "start id : " + scdid;
    Poco::Logger::root().information(log, __FILE__, __LINE__);

    std::cout << url << std::endl;

    auto callback = std::make_shared<std::function<void(cpr::Response)>>(
        [](cpr::Response r) {
            if (r.status_code == 200)
            {
            }
            else
            {
            }
        }
    );

    cpr::Response r;
    if (method == "get")
    {
        cpr::GetCallback(*callback,param);

    }else 
    if (method == "post")
    {
        cpr::PostCallback(*callback,param);
    }else 
    if (method == "put")
    {
        cpr::PutCallback(*callback,param);
    }else 
    if (method == "delete")
    {
        cpr::DeleteCallback(*callback,param);
    }
}
void schedule::registration::tryFunc()
{
    std::string log = "trying id : " + scdid;
    Poco::Logger::root().information(log, __FILE__, __LINE__);

    cpr::Response r;
    if (method == "get")
    {
        r = cpr::Get(param);
    }
    else
    if (method == "post")
    {
        r = cpr::Post(param);
    }else 
    if (method == "put")
    {
        cpr::Put(param);
    }else 
    if (method == "delete")
    {
        cpr::Delete(param);
    }

    std::string _stt;

    if (r.status_code == 200)
    {
        std::cout << r.text <<std::endl ; 
    }
    else
    {
        std::cout << r.text << std::endl;
    }
}