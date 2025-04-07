#pragma once

//#include <string>
//#include <map>
//
//// Poco.
//#include "Poco/JWT/Token.h"
//#include "Poco/JWT/Signer.h"
//#include "Poco/JWT/Serializer.h"
//#include "Poco/JWT/JWTException.h"
//#include "Poco/DateTime.h"
//#include "Poco/DateTimeFormatter.h"
//#include "Poco/DateTimeFormat.h"
//#include "Poco/LogFile.h"
//#include "Poco/Path.h"
//#include "Poco/File.h"
//#include "Poco/Delegate.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//// stringprocess 
//#include "Source/Tools/StringProcess.h"
//
//// cpr 
//#include <cpr/cpr.h>
//#include <cpr/multipart.h>

#include "Token/Token.h"

class product
{
public:
	product() {};
	product(std::string _type, std::string _name, std::string _config, std::string _secretKey, std::string _id = "");
	~product() {};

private:
	std::string type;
	std::string name;
	std::string config;
public:
	static tokenManager m_TokenManager;

public:

	static std::string host;
	static std::string authorization;
	static std::string id;
public:
	void setHost(std::string _host);
	void setAuthorization(std::string _authorization);
public:
	static BOOL getProduct(std::string productName);
};
