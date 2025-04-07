#pragma once
#include "pch.h"
#include "Token.h"
#include "Authorization/Authorization.h"

tokenManager::tokenManager()
{
}
BOOL tokenManager::Encode(const std::string& jwt)
{
	return 0;
}
int tokenManager::CheckAccessToken(const std::string& AccessToken)
{
	token tmp;
	tmp.setSecretKey(secretKey);

	return tmp.Verify(AccessToken);
}
BOOL tokenManager::getListAutho(const std::string& AccessToken, std::map<std::string, authorize>& listAutho, std::string& productid)
{
	token tmp;
	tmp.setSecretKey(secretKey);
	std::map<std::string, std::string> Payload;

	if (tmp.getPayload(AccessToken, Payload))   /// check dung jwt , expire .... 
	{
		if (Payload.find("productid") == Payload.end()) return 0;
		productid = Payload["productid"];

		/*if (Payload.find("user_name") == Payload.end()) return 0;
		user_name = Payload["user_name"]; */

		if (Payload.find("authorization") == Payload.end()) return 0;

		listAutho.clear();

		try
		{
			json json = MOVE(json::parse(Payload["authorization"]));
			for (auto x : json)
			{
				//std::string productid = x["productid"];
				std::string roleid = x["roleid"];
				std::string userid = x["userid"];
				std::string id = x["id"];
				listAutho[id] = authorize(userid, roleid, productid, id);
			}
		}
		catch (const std::exception&)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}
BOOL tokenManager::checkValidate(const std::string& AccessToken, std::string productid)
{
	std::string sURL = host + "/token/v1/validate";
	json body;
	body["productid"] = productid;
	body["token"] = AccessToken;

	cpr::Response r = cpr::Post(cpr::Url{ sURL }, cpr::Body{ body.dump() }, cpr::Timeout(1500), cpr::ConnectTimeout(1500));

	if (r.text != "")
	{
		rapidjson::Document document;
		if (document.Parse(r.text.c_str()).HasParseError())
		{
			return 201;
		}
		assert(document.IsObject());
		const rapidjson::Value& code = document["code"];

		if (code.GetInt() != 200)
		{
			return code.GetInt();
		}
		else return 200;
	}
	return 201;
}