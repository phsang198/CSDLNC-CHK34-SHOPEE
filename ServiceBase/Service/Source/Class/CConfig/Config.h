#pragma once
#include "ConnectionPool/CrowRequestPool.h"

class Config
{
public:
	Config();
	~Config();
public:
	static std::string VBDURL;
	static std::string VBDKey;

public:
	static std::string getConfig();
	static BOOL setConfig(const std::string& uConfig);
public:
	static BOOL loadServiceConfig(int& servicePort, int& maxThread, int& maxQueue, int& timeOut);
	static BOOL loadDBConfig(std::string& dbName, std::string& dbHost, int& dbPort, std::string& dbUser, std::string& dbPassword);

	static BOOL loadTokenConfig(std::string& host);
	static BOOL loadSMeshConfig(std::string& host, std::string& config);

	static BOOL loadErrorConfig();
private:
	static BOOL parse();
public:
	static BOOL readFileConfig(std::string path, std::string& content);
private:
	static std::string _config;

};


