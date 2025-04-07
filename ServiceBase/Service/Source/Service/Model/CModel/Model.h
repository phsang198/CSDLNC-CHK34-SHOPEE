#pragma once
#include "ConnectionPool/ConnectionPool.h"

// token manager
#include "Token/Token.h"
#include "User/User.h"
#include "Product/Product.h"
#include "Authorization/Authorization.h"
#include "Role/Role.h"

class CModel
{
public: 
	CModel(); 
	~CModel(); 
public:
	std::string msg;
public: 
	BOOL Connect(std::string dbName, std::string dbHost, int dbPort, std::string dbUser, std::string dbPassword);  
	BOOL initSchema(std::string content);
	//----------------------------------------------------------------------------------------------------------------------
	void Destroy();
	
public: 
	//int m_CacheSize; //Cache Size
};

