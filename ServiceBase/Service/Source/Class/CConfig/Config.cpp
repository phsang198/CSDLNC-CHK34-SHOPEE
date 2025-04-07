#pragma once 
#include "pch.h"
#include "Config.h"

std::string Config::_config;
rapidjson::Document document;

std::string Config::VBDURL = "";
std::string Config::VBDKey = "";

BOOL Config::parse()
{
	if (document.Parse(_config.c_str()).HasParseError())
	{
		Poco::Logger::root().information("Can't open Config file", __FILE__, __LINE__);
		return FALSE;
	}
	assert(document.IsObject());
	return TRUE;
}

Config::Config()
{
	auto t = getConfig();
}

Config::~Config()
{
}

std::string Config::getConfig()
{
	if (_config != "") return _config;

	std::string path = SupportFunc::getExePath(RUN_PROJECT);
	if (!readFileConfig(path + FILE_CONFIG_PATH, _config))
	{
		Poco::Logger::root().information("Can't open Config file", __FILE__, __LINE__);
		return "";
	}
	if (!parse()) return "";

	return _config;
}

BOOL Config::setConfig(const std::string& uConfig)
{
	std::string save = _config;
	_config = uConfig;

	if (!parse())
	{
		_config = save;
		return FALSE;
	}
	return TRUE;
}

BOOL Config::readFileConfig(std::string path, std::string& content)
{
	std::ifstream f;
	f.open(path, std::ios::in);
	std::string tmp;
	if (f.fail())
	{
		return FALSE;
	}
	while (std::getline(f, tmp)) {
		content += (tmp + '\n');
	}
	f.close();
	return TRUE;
}

BOOL Config::loadServiceConfig(int& servicePort, int& maxThread, int& maxQueue, int& timeOut)
{
	rapidjson::Value::Object service = document["service"].GetObject();

	servicePort = service["port"].GetInt();
	maxThread = service["crow"]["maxThread"].GetInt();
	maxQueue = service["crow"]["maxQueue"].GetInt();
	timeOut = service["crow"]["timeOut"].GetInt();

	return TRUE;
}

BOOL Config::loadDBConfig(std::string& dbName, std::string& dbHost, int& dbPort, std::string& dbUser, std::string& dbPassword)
{
	const rapidjson::Value& components = document["components"];

	for (const auto& component : components.GetArray())
	{
		if (std::string(component["name"].GetString()) == "PostgreSQL")
		{
			auto para = component["parameters"].GetObject();
			dbName = para["database_name"].GetString();
			dbHost = para["host"].GetString();
			dbPort = para["port"].GetInt();
			dbUser = para["user"].GetString();
			dbPassword = para["password"].GetString();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Config::loadTokenConfig(std::string& host)
{
	const rapidjson::Value& components = document["components"];

	for (const auto& component : components.GetArray())
	{
		if (std::string(component["name"].GetString()) == "Token")
		{
			std::string _host = component["parameters"]["host"].GetString();
			std::string port = std::to_string(component["parameters"]["port"].GetInt());
			host = "http://" + _host + ":" + port;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Config::loadSMeshConfig(std::string& host, std::string& config)
{
	rapidjson::Value& components = document["components"];
	auto& allocator = document.GetAllocator();
	for (auto& component : components.GetArray())
	{
		if (std::string(component["name"].GetString()) == "Mesh")
		{
			std::string _host = component["parameters"]["host"].GetString();
			std::string port = std::to_string(component["parameters"]["port"].GetInt());
			host = "http://" + _host + ":" + port;

			rapidjson::Document copy_doc2;
			auto& allocator1 = copy_doc2.GetAllocator();

			copy_doc2.CopyFrom(document, allocator1);

			copy_doc2["service"].RemoveMember("name");
			copy_doc2["service"].RemoveMember("port");
			copy_doc2["service"].RemoveMember("app_token");
			copy_doc2["service"].RemoveMember("description");
			copy_doc2["service"].RemoveMember("service_guid");
			copy_doc2["service"].RemoveMember("service_type");

			rapidjson::Document copy_doc;
			auto& allocator2 = copy_doc.GetAllocator();

			copy_doc.CopyFrom(document["service"], allocator2);
			copy_doc.RemoveMember("crow");
			copy_doc.RemoveMember("features");

			copy_doc.AddMember("config", copy_doc2, allocator1);
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			copy_doc.Accept(writer);
			config = buffer.GetString();

			return TRUE;
		}
	}
	return FALSE;
}

BOOL Config::loadErrorConfig()
{
	const rapidjson::Value& components = document["components"];

	for (const auto& component : components.GetArray())
	{
		if (std::string(component["type"].GetString()) == "Error code")
		{
			const rapidjson::Value& error = component["parameters"]["error"];

			for (const auto& object : error.GetArray())
			{
				int code = object["code"].GetInt();
				std::string name = object["name"].GetString();

				lstError[code] = name;
			}
			return TRUE;
		}
	}
	return FALSE;
}