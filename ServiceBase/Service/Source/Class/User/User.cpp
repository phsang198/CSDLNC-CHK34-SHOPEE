#pragma once
#include "pch.h"
#include "User.h"

User::User(std::string _user_name, std::string _password, std::string _email, int _type)
{
	user_name = _user_name;
	password = _password;
	email = _email;
}

User::User(std::string _user_name, std::string _password, std::string _name, std::string _email, std::string _phone_number, std::string _avatar,
	std::string _addresss, std::string _id)
{
	 id = _id;
	 user_name = _user_name;
	 password = _password; 
	 name = _name;
	 email = _email;
	 phone_number = _phone_number;
	 avatar = _avatar;
	 address = _addresss;
}
BOOL User::checkUserName(std::string msg)
{
	std::regex pattern("^[a-zA-Z][a-zA-Z0-9]*$");

	if (std::regex_match(user_name, pattern))
	{
		return 1;
	}
	return 0;
}

BOOL User::checkPassWord(std::string msg)
{
	std::regex pattern("^(?=^[A-Za-z])(?=.{8,})(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=[\\w]*[!@#$%^&*()_+=][\\w]*$).*$");

	if (std::regex_match(password, pattern))
	{
		return 1;
	}
	return 0;
}

BOOL User::checkEmail(std::string msg)
{
	const std::regex pattern("^[_a-z0-9-]+(.[_a-z0-9-]+)*@[a-z0-9-]+(.[a-z0-9-]+)*(.[a-z]{2,4})$");
	return std::regex_match(email, pattern);
}

