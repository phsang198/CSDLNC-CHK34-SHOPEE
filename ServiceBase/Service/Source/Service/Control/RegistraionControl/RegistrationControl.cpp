#pragma once 
#include "pch.h"
#include "Control/MainControl/IDNService.h" 
#include "schedule/schedule.h"
#include "UUID/UuidGenerator.h"

nlohmann::json CIDNService::GenerateSchedule2JSON(const std::vector<RESOURCETYPE>& reg)
{
	json lst = json::array();
	for (auto& resource : reg)
	{
		std::shared_ptr<schedule::registration> ws = std::get<std::shared_ptr<schedule::registration>>(resource);
		json kq;

		if (ws->body != "")
		{
			auto req = schedule::manager::getFrequent(ws->scdid);
			if (req)
			{
				kq[RP_SCHEDULE_PARAM_STATUS] = req->getState(); 
			}
			kq.merge_patch(json::parse(ws->body)); 
			kq["created"] = ws->created; 
			kq[RP_SCHEDULE_PARAM_ID] = ws->scdid;
		}
		else
		{
			kq[RP_SCHEDULE_PARAM_ID] = ws->scdid;
			kq[RP_SCHEDULE_PARAM_SCHEDULENAME] = ws->scdname;
			kq[RP_SCHEDULE_PARAM_WORKFLOWID] = ws->wid;
			kq[RP_SCHEDULE_PARAM_INSTANCE_PTIME] = ws->itimeout;
			kq[RP_SCHEDULE_PARAM_WORKFLOW_PTIME] = ws->wtimeout;

			json lst_i = json::array();
			for (auto& is : ws->li)
			{
				json kq2;

				kq2[RP_SCHEDULE_PARAM_INPUT] = json::parse(is->input);
				kq2[RP_SCHEDULE_PARAM_INSTANCEID] = is->iid;
				kq2[RP_SCHEDULE_PARAM_STARTTIME] = is->starttime;
				kq2[RP_SCHEDULE_PARAM_STATUS] = is->status;
				lst_i.push_back(kq2);
			}
			kq[RP_SCHEDULE_PARAM_INSTANCES] = lst_i;
		}
		lst.push_back(kq);
	}
	json res;
	res[RP_SCHEDULE] = lst;
	return res;
}

void CIDNService::API_SCHEDULE_GET_METHOD() 
{
	CROW_ROUTE(app, RQ_SCHEDULE_REGISTER_URL).methods("GET"_method)
		([this](const crow::request& req)
			{
				crow::response res;  
				auto& ctx = app.get_context<SecurityMiddleware>(req);

				std::string type = uGetVal(3, res, uParam, RQ_SCHEDULE_PARAM_TYPE);
				std::string id = uGetVal(6, res, uParam, RQ_SCHEDULE_PARAM_ID);

				if (res.code == BAD_REQUEST) { res.code = OK; return res; }

				int error_code = 200; 
				json kq;

				if (id == "")
				{
					std::vector<RESOURCETYPE> reg;
					error_code = m_ResourceModel.getResource("schedule", ctx.auid, reg);
					if (error_code == 200)
					{
						kq = GenerateSchedule2JSON(reg);
					}
				}
				else if (type == "status" )
				{
					std::vector<std::string> arr = StringProcess::parseURLToArray(uParam.get(RQ_SCHEDULE_PARAM_ID), ',');

					json lst = json::array();
					for (auto& x : arr)
					{
						json tmp; 
						{
							tmp["id"] = x;
							tmp["status"] = schedule::manager::getstatus(x);
						}
						lst.push_back(tmp);
					}
					if (!lst.empty())
					{
						error_code = 200;
						kq["schedule_status"] = lst;
					}
					else error_code = 303;
				}
				return RESP(error_code, kq);

			});
	CROW_ROUTE(app, RQ_SCHEDULE_EXCUTE_URL).methods("GET"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				auto& ctx = app.get_context<SecurityMiddleware>(req);

				std::string type = uGetVal(3, res, uParam, RQ_SCHEDULE_PARAM_TYPE);
				std::string id = uGetVal(6, res, uParam, RQ_SCHEDULE_PARAM_ID);

				if (res.code == BAD_REQUEST) { res.code = OK; return res; }
				
				std::string auid = ctx.auid; 
				if (ctx.roleid == "60612643-cb8c-4fc6-acd6-3f3019faf451")
					auid = ctx.roleid; 
				int error_code = 200;
				json kq;

				if (id == "")
				{
					std::vector<RESOURCETYPE> reg;
					error_code = m_ResourceModel.getResource("excute_schedule", auid, reg);
					if (error_code == 200)
					{
						kq = GenerateSchedule2JSON(reg);
					}
				}
				return RESP(error_code, kq);

			});
}
void CIDNService::API_SCHEDULE_POST_METHOD()
{
	CROW_ROUTE(app, RQ_SCHEDULE_REGISTER_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}

				auto& ctx = app.get_context<SecurityMiddleware>(req);

				auto reg = std::make_shared<schedule::registration>();

				reg->scdname = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_SCHEDULENAME);
				reg->wid = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_WORKFLOWID);
				std::string itimeout = bGetVal(2, res, x, RQ_SCHEDULE_PARAM_INSTANCE_PTIME);
				std::string wtimeout = bGetVal(2, res, x, RQ_SCHEDULE_PARAM_WORKFLOW_PTIME);

				std::string ins = bGetVal(4, res, x, RQ_SCHEDULE_PARAM_INSTANCES);

				if (res.code != OK) { res.code = OK; return res; }


				reg->itimeout = std::stoi(itimeout);
				reg->wtimeout = std::stoi(wtimeout);

				json body = json::parse(req.body); 

				for (auto& inst : body[RQ_SCHEDULE_PARAM_INSTANCES])
				{
					std::string input = inst[RQ_SCHEDULE_PARAM_INPUT].dump();
					std::string iid = inst[RQ_SCHEDULE_PARAM_INSTANCEID];
					std::string sstarttime = inst[RQ_SCHEDULE_PARAM_STR_STARTTIME];
					int starttime; 
					if (sstarttime != "") starttime = ToEpochTime::convertToEpoch(sstarttime);
					else starttime = inst[RQ_SCHEDULE_PARAM_STARTTIME];
					

					auto is = std::make_shared<schedule::sinstance>(iid, input, starttime); 
					inst["status"] = is->status;

					is->token = ctx.token; 
					is->itimeout = reg->itimeout; 

					reg->li.push_back(std::move(is));
				}
				reg->inss = body[RQ_SCHEDULE_PARAM_INSTANCES].dump();
				

				int error_code = m_ScheduleModel.addSchedule(reg);

				if (error_code == 200)
				{
					resource _resource(ctx.auid, "schedule", reg->scdid, "", "");
					m_ResourceModel.addResource(_resource);

					kq["id"] = reg->scdid;

					schedule::manager::add(std::move(reg));

				}
				return RESP(error_code, kq);
			});
	CROW_ROUTE(app, RQ_SCHEDULE_EXCUTE_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}

 				std::string url = bGetVal(3, res, x, "url");
				std::string method = bGetVal(3, res, x, "method");
				std::string Trigger_Interval = bGetVal(3, res, x, "Trigger_Interval");
				std::string name  = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_SCHEDULENAME);

				if (res.code != OK) { res.code = OK; return res; }

				std::string body = bGetVal(4, res, x, "body");
				if (res.code == BAD_REQUEST) { res.code = OK; return res; }

				std::string token = bGetVal(3, res, x, "token");
				if (res.code == BAD_REQUEST) { res.code = OK; return res; }
				res.code = OK;

				json tmp_p = json::parse(req.body);
				if (tmp_p.count("body") != 0)
					body = tmp_p["body"].dump();

				ToEpochTime tept; 
				tept.Trigger_Interval = Trigger_Interval; 
				if (Trigger_Interval == "Seconds")
				{
					std::string sbt = bGetVal(2, res, x, "Seconds_between_Triggers");
					if (res.code != OK) { res.code = OK; return res; }
					tept.sTrigger = ToEpochTime::STrigger({ std::stoi(sbt) });
				}
				else if (Trigger_Interval == "Minutes")
				{
					std::string mbt = bGetVal(2, res, x, "Minutes_between_Triggers");
					if (res.code != OK) { res.code = OK; return res; }
					tept.mTrigger = ToEpochTime::MTrigger({ std::stoi(mbt) });
				}
				else if (Trigger_Interval == "Hours")
				{
					std::string hbt = bGetVal(2, res, x, "Hours_between_Triggers");
					std::string tam = bGetVal(2, res, x, "Trigger_at_Minute");
					if (res.code != OK) { res.code = OK; return res; }
					tept.hTrigger = ToEpochTime::HTrigger({ std::stoi(hbt),std::stoi(tam) });
				}
				else if (Trigger_Interval == "Days")
				{
					std::string dbt = bGetVal(2, res, x, "Days_between_Triggers");
					std::string tah = bGetVal(2, res, x, "Trigger_at_Hour");
					std::string tam = bGetVal(2, res, x, "Trigger_at_Minute");
					if (res.code != OK) { res.code = OK; return res; }
					tept.dTrigger = ToEpochTime::DTrigger({ std::stoi(dbt),std::stoi(tah),std::stoi(tam) });
				}
				else if (Trigger_Interval == "Timepoint")
				{
					tept.startTime = bGetVal(3, res, x, "Start_Time");
					if (res.code != OK) { res.code = OK; return res; }
				}
				else
				{
					json jError; 
					jError["key"] = "Trigger_Interval"; 
					jError["detail"] = "invalid Trigger Interval"; 
					return RESP(201,kq, jError);
				}

				std::shared_ptr<schedule::registration> reg = std::make_shared<schedule::registration>
															  (url, method,body, token, tept);

				int error_code = m_ScheduleModel.addExcuteSchedule(name,req.body,reg->scdid);

				if (error_code == 200)
				{
					auto& ctx = app.get_context<SecurityMiddleware>(req);

					resource _resource(ctx.auid, "excute_schedule", reg->scdid, "", "");
					m_ResourceModel.addResource(_resource);

					kq["id"] = reg->scdid;

					schedule::manager::initFrequent(reg);
					
				}
				return RESP(error_code, kq);
			});
	CROW_ROUTE(app, RQ_SCHEDULE_STOP_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				std::string id = uGetVal(3, res, uParam, RQ_SCHEDULE_PARAM_ID);
				if (res.code != OK) { res.code = OK; return res; }
		
				return RESP(schedule::manager::stopFrequent(id));
			});
	CROW_ROUTE(app, RQ_SCHEDULE_START_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				std::string id = uGetVal(3, res, uParam, RQ_SCHEDULE_PARAM_ID);
				if (res.code != OK) { res.code = OK; return res; }

				return RESP(schedule::manager::restartFrequent(id));
			});
	CROW_ROUTE(app, RQ_SCHEDULE_TRY_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				std::string id = uGetVal(3, res, uParam, RQ_SCHEDULE_PARAM_ID);
				if (res.code != OK) { res.code = OK; return res; }

				return RESP(schedule::manager::tryFunc(id));
			});
}
void CIDNService::API_SCHEDULE_PUT_METHOD()
{
	CROW_ROUTE(app, RQ_SCHEDULE_REGISTER_URL).methods("PUT"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}
				auto reg = std::make_shared<schedule::registration>();

				reg->scdname = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_SCHEDULENAME);
				reg->wid = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_WORKFLOWID);
				std::string itimeout = bGetVal(2, res, x, RQ_SCHEDULE_PARAM_INSTANCE_PTIME);
				std::string wtimeout = bGetVal(2, res, x, RQ_SCHEDULE_PARAM_WORKFLOW_PTIME);

				std::string ins = bGetVal(4, res, x, RQ_SCHEDULE_PARAM_INSTANCES);

				if (res.code != OK) { res.code = OK; return res; }


				reg->itimeout = std::stoi(itimeout);
				reg->wtimeout = std::stoi(wtimeout);

				json body = json::parse(req.body); 

				for (auto& inst : body[RQ_SCHEDULE_PARAM_INSTANCES])
				{
					std::string input = inst[RQ_SCHEDULE_PARAM_INPUT].dump();
					std::string iid = inst[RQ_SCHEDULE_PARAM_INSTANCEID];
					int starttime = inst[RQ_SCHEDULE_PARAM_STARTTIME];
					
					if (res.code != OK) { res.code = OK; return res; }

					auto is = std::make_shared<schedule::sinstance>(iid, input, starttime); 
					inst["status"] = is->status;

					reg->li.push_back(std::move(is));
				}
				reg->inss = body[RQ_SCHEDULE_PARAM_INSTANCES].dump();
				

				int error_code = m_ScheduleModel.addSchedule(reg);

				if (error_code == 200)
				{
					/*auto& ctx = app.get_context<SecurityMiddleware>(req);
					resource _resource(ctx.auid, "workflow", _wf.id, "", "");
					m_ResourceModel.addResource(_resource);*/

					kq["id"] = reg->scdid;

					//schedule::manager::m_schedule[reg->wid][reg->scdid].push_back(std::move(reg));
				}
				return RESP(error_code, kq);
			});

	CROW_ROUTE(app, RQ_SCHEDULE_EXCUTE_URL).methods("PUT"_method)
		([this](const crow::request& req)
			{
				crow::response res;
				json kq;

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}

				std::string id = bGetVal(3, res, x, "id");
				std::string url = bGetVal(3, res, x, "url");
				std::string method = bGetVal(3, res, x, "method");
				std::string Trigger_Interval = bGetVal(3, res, x, "Trigger_Interval");
				std::string name = bGetVal(3, res, x, RQ_SCHEDULE_PARAM_SCHEDULENAME);

				if (res.code != OK) { res.code = OK; return res; }

				std::string body = bGetVal(4, res, x, "body");
				if (res.code == BAD_REQUEST) { res.code = OK; return res; }

				std::string token = bGetVal(3, res, x, "token");
				if (res.code == BAD_REQUEST) { res.code = OK; return res; }
				res.code = OK;

				json tmp_p = json::parse(req.body);
				if (tmp_p.count("body") != 0)
					body = tmp_p["body"].dump();

				ToEpochTime tept;
				tept.Trigger_Interval = Trigger_Interval;
				if (Trigger_Interval == "Seconds")
				{
					std::string sbt = bGetVal(2, res, x, "Seconds_between_Triggers");
					if (res.code != OK) { res.code = OK; return res; }
					tept.sTrigger = ToEpochTime::STrigger({ std::stoi(sbt) });
				}
				else if (Trigger_Interval == "Minutes")
				{
					std::string mbt = bGetVal(2, res, x, "Minutes_between_Triggers");
					if (res.code != OK) { res.code = OK; return res; }
					tept.mTrigger = ToEpochTime::MTrigger({ std::stoi(mbt) });
				}
				else if (Trigger_Interval == "Hours")
				{
					std::string hbt = bGetVal(2, res, x, "Hours_between_Triggers");
					std::string tam = bGetVal(2, res, x, "Trigger_at_Minute");
					if (res.code != OK) { res.code = OK; return res; }
					tept.hTrigger = ToEpochTime::HTrigger({ std::stoi(hbt),std::stoi(tam) });
				}
				else if (Trigger_Interval == "Days")
				{
					std::string dbt = bGetVal(2, res, x, "Days_between_Triggers");
					std::string tah = bGetVal(2, res, x, "Trigger_at_Hour");
					std::string tam = bGetVal(2, res, x, "Trigger_at_Minute");
					if (res.code != OK) { res.code = OK; return res; }
					tept.dTrigger = ToEpochTime::DTrigger({ std::stoi(dbt),std::stoi(tah),std::stoi(tam) });
				}
				else if (Trigger_Interval == "Timepoint")
				{
					tept.startTime = bGetVal(3, res, x, "Start_Time");
					if (res.code != OK) { res.code = OK; return res; }
				}
				else
				{
					json jError;
					jError["key"] = "Trigger_Interval";
					jError["detail"] = "invalid Trigger Interval";
					return RESP(201, kq, jError);
				}

				std::shared_ptr<schedule::registration> reg = std::make_shared<schedule::registration>
					(url, method, body, token, tept, id);

				int error_code = m_ScheduleModel.updateExcuteSchedule(name, req.body, id);

				if (error_code == 200)
				{
					schedule::manager::deleteExcute(id);
					schedule::manager::initFrequent(reg);
				}
				return RESP(error_code);
			});
}
void CIDNService::API_SCHEDULE_DELETE_METHOD()
{
	CROW_ROUTE(app, RQ_SCHEDULE_REGISTER_URL).methods("DELETE"_method)
		([this](const crow::request& req)
			{
				crow::response res;

				std::string scdid = uGetVal(3,res, uParam, RQ_SCHEDULE_PARAM_SCHEDULEID);
				//std::string iid = uGetVal(3,res, uParam, RQ_SCHEDULE_PARAM_INSTANCEID);
				if (res.code == BAD_REQUEST) { res.code = OK; return res; }

				int error_code = m_ScheduleModel.deleteSchedule(scdid);
				if (error_code == 200)
				{
					schedule::manager::deleteIs(scdid);
				}

				return RESP(error_code);
			});

	CROW_ROUTE(app, RQ_SCHEDULE_EXCUTE_URL).methods("DELETE"_method)
		([this](const crow::request& req)
			{
				crow::response res;

				std::string scdid = uGetVal(3, res, uParam, "id");
				if (res.code == BAD_REQUEST) { res.code = OK; return res; }

				auto& ctx = app.get_context<SecurityMiddleware>(req);

				int error_code;
				if (ctx.roleid == roleManager::lst_role_name["admin system"])
				{
					error_code = m_ScheduleModel.deleteExcuteSchedule(scdid);
				}
				else
				{
					error_code = m_ResourceModel.deleteResource("excute_schedule",ctx.auid,scdid);
				}
				if (error_code == 200)
				{
					schedule::manager::deleteExcute(scdid);
				}
				return RESP(error_code);
			});
}
void CIDNService::API_SCHEDULE()
{
	API_SCHEDULE_GET_METHOD();
	API_SCHEDULE_POST_METHOD(); 
	API_SCHEDULE_PUT_METHOD();
	API_SCHEDULE_DELETE_METHOD();
}