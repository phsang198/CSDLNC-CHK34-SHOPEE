#pragma once

class User; 

class UserModel 
{
public:
	UserModel();
	~UserModel();
public:
	std::string msg;
public:
	//-------------------------------------------------------------------------------------------------------------------------
	int hasUser(User& _user, const std::string& key);
	int getID(User& _user);
	int processLink(User& _user);
	int processRegister(User& _user);
	int createUser(User& _user);
	int updateUser(int type, User _user, std::string new_password);
	int getUser(std::string type, std::vector<User>& _user, std::string auid = "");
	int deleteUser(std::string user_name, std::string id);
	//-------------------------------------------------------------------------------------------------------------------------

private:

};

