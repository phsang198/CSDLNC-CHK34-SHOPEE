#pragma once 
#include "pch.h"
#include <iomanip>
#include <sstream>
#include <chrono>

#include "CConfig/Config.h"
#include "Control/MainControl/IDNService.h" 

void CIDNService::API_SERVICEMESH()
{
	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/healthy").methods("GET"_method)
		([this](const crow::request& req)
			{
				json kq;
				kq["status"] = "OK";
				kq["requested_at"] = mesh::getISO8601Timestamp();

				std::ostringstream oss;
				oss << kq;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});
	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/config").methods("POST"_method)
		([this](const crow::request& req)
			{
				crow::response res;

				auto x = crow::json::load(req.body);
				if (!x)
				{
					return RESP(150);
				}
				return RESP(200);
			});
	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/config").methods("GET"_method)
		([this](const crow::request& req)
			{
				json json;
				json["summary"].merge_patch(json::parse(mesh::config));

				json["summary"]["created"] = mesh::created;
				json["summary"]["config"] = json::parse(Config::getConfig());
				json["summary"]["host"] = mesh::thishost;
				json["status"] = "OK";
				json["requested_at"] = mesh::getISO8601Timestamp();

				std::ostringstream oss;
				oss << json;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});
}