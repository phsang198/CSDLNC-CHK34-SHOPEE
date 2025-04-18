#pragma once
#include "ConnectionPool/ConnectionPool.h"

// token manager
#include "User/User.h"

class CModel
{
public: 
	CModel(); 
	~CModel(); 
public:
	std::string msg;
public: 
	bool Connect(std::string dbName, std::string dbHost, int dbPort, std::string dbUser, std::string dbPassword);  
	bool initSchema(std::string content);
	//----------------------------------------------------------------------------------------------------------------------
	void Destroy();
	
public: 
	//int m_CacheSize; //Cache Size
};

