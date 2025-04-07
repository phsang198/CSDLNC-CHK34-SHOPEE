#pragma once 
#include "pch.h"
#include "Control/MainControl/IDNService.h" 
#include "Model/ConnectionModel/ConnectionModel.h"

void CIDNService::API_NETWORK()
{
	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, RQ_NETWORK_URL).methods("GET"_method)
	([this](const crow::request& req)
	{
		crow::response res;

		json kq;
		kq["connections"] = ConnectionModel::checkConnection(); 
		return RESP(200, kq);

	});
}