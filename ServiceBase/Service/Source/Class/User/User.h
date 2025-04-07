#pragma once
#include "Tools/StringProcess.h"
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <assert.h>
//===========================================================================================================
class User
{
public:
	User()
	{
	}
	User(std::string _user_name, std::string _password, std::string _email,int _type=-1);
	User(std::string _user_name, std::string _password, std::string _name = "", std::string _email = "", std::string _phone_number = "", std::string _avatar = "", std::string _address = "", std::string _id = "");
	~User()
	{
	}
public:
	std::string id;
	std::string user_name;
	std::string password;
	std::string name;
	std::string email;
	std::string phone_number;
	std::string avatar;
	std::string building;
	std::string address;

public:
	BOOL checkUserName(std::string msg = "");
	BOOL checkPassWord(std::string msg = "");
	BOOL checkEmail(std::string msg = "");
};

