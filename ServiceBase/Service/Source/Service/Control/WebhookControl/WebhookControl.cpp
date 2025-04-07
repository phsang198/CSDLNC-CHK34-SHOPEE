#pragma once 
#include "pch.h"
#include "Control/MainControl/IDNService.h" 

#include "schedule/schedule.h"

void CIDNService::API_WEBHOOK()
{
	CROW_ROUTE(app, RQ_WEBHOOK_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;

				Poco::Logger::root().information(req.body, __FILE__, __LINE__);

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}
				json body = json::parse(req.body); 


				std::string event = body["wheid"]; 
				if (event == "e3c92b6a-6d27-43d5-81c8-b426b8080b1f")
				{
					Poco::Logger::root().information("Start func - stt: Event oke", __FILE__, __LINE__);

					std::string iid = body["payloads"]["iid"];
					std::string wid = body["payloads"]["wid"];
					std::string status = body["payloads"]["status"];
					if (status == "SAVE_PROCESS") status = "Completed";
					else if (status == "SUSPEND") status = "Terminated";
					auto reg = schedule::manager::updateIStatus(wid, iid, status); 
					m_ScheduleModel.updateSchedule(reg,1); 
				}
			
				return RESP(200);
			}); 
	//------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, RQ_TEST_URL).methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}
				std::string guid = bGetVal(3, res, x, "guid");
				std::string wkb = bGetVal(3, res, x, "wkb");
				m_ResourceModel.Query(guid,wkb);
				return RESP(200);
			});

}