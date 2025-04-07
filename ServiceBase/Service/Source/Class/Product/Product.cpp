#include "pch.h"
#include "Product.h"

tokenManager product::m_TokenManager;
std::string product::host;
std::string product::authorization;
std::string product::id;

product::product(std::string _type, std::string _name, std::string _config, std::string _secretKey, std::string _id)
{
	type = _type;
	name = _name;
	config = _config;
	m_TokenManager.secretKey = _secretKey;
	id = _id;
}

void product::setHost(std::string _host)
{
	host = _host;
}

void product::setAuthorization(std::string _authorization)
{
	authorization = _authorization;
}

BOOL product::getProduct(std::string productName)
{
	std::string sURL = host + "/token/v1/product?service=$name";
	StringProcess::ReplaceAll(sURL, "$name", productName);
	cpr::Response r = cpr::Get(cpr::Url{ sURL }, cpr::Header{ {"token",authorization} }, cpr::Timeout(1000), cpr::ConnectTimeout(1500));

	if (r.text != "")
	{
		rapidjson::Document document;
		if (document.Parse(r.text.c_str()).HasParseError())
		{
			return FALSE;
		}
		assert(document.IsObject());
		const rapidjson::Value& code = document["code"];

		if (code.GetInt() != 200) return FALSE;

		auto arr = document["data"]["product"].GetArray(); 
		for (rapidjson::Value::ConstValueIterator itr = arr.Begin(); itr != arr.End(); ++itr)
		{
			auto _product = itr->GetObjectW();

			std::string secretKey = _product["secretkey"].GetString();
			std::string type = _product["type"].GetString();
			std::string _id = _product["id"].GetString();

			auto config = _product["config"].GetObjectW();
			/*product *tmp = (new product(type, productName, "config", secretKey, id));
			this-> = tmp;*/
			m_TokenManager.secretKey = secretKey;
			m_TokenManager.host = host;
			id = _id;
			return TRUE;
		}
	}
	return FALSE;
}
