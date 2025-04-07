#pragma once 
#include "pch.h"
#include "UserModel.h"
#include "ConnectionPool/ConnectionPool.h"
#include "User/User.h"


UserModel::UserModel()
{
}
UserModel::~UserModel()
{
}
//-------------------------------------------------------------------------------------------------------------------------
int UserModel::hasUser(User& _user, const std::string& key)
{
	std::string query = "";
	if (key == "user_name")
	{
		query = "SELECT * FROM \"user\" WHERE user_name = '$user_name'";
		StringProcess::Replace(query, "$user_name", _user.user_name);
	}
	if (key == "email")
	{
		query = "SELECT * FROM \"user\" WHERE email = '$email'";
		StringProcess::Replace(query, "$email", _user.email);
	}
	if (key == "password")
	{
		query = "SELECT * FROM \"user\" WHERE user_name = '$user_name' AND password = '$password'";
		StringProcess::Replace(query, "$user_name", _user.user_name);
		StringProcess::Replace(query, "$password", _user.password);
	}
	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		poFeature = poLayer->GetNextFeature();
		std::string id = poFeature->GetFieldAsString("id");
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int UserModel::getID(User& _user)
{
	std::string query = "SELECT * FROM \"user\" WHERE user_name = '$user_name' AND password = '$password'";
	StringProcess::Replace(query, "$user_name", _user.user_name);
	StringProcess::Replace(query, "$password", _user.password);

	return ConnectionManager::QueryOrther(query, _user.id);
}
int UserModel::processLink(User& _user)
{
	if (hasUser(_user, "user_name") != 200)
	{
		return 110;
	}
	if (hasUser(_user, "password") != 200)
	{
		return 115;
	}
	getID(_user); 
	return 200; 
}
int UserModel::processRegister(User& _user)
{
	/*if (_user.user_name == _user.password)
	{
		return 114;
	}*/
	if (hasUser(_user, "user_name") == 200)
	{
		return 112;
	}
	if (hasUser(_user, "email") == 200)
	{
		return 113;
	}
	if (!_user.checkUserName())
	{
		return 107;
	}
	if (!_user.checkEmail())
	{
		return 108;
	}
	/*if (!_user.checkPassWord())
	{
		return 115;
	}*/
	return createUser(_user);
}
int UserModel::createUser(User& _user)
{
	std::string query = 
		R"(INSERT INTO "user" (user_name,password,name,email,phone_number,avatar,address)
			VALUES('$user_name', '$password', '$name', '$email', '$phone_number', 
					decode('$avatar', 'base64'), '$address') 
			RETURNING id; )";

	StringProcess::Replace(query, "$user_name", _user.user_name);
	StringProcess::Replace(query, "$password", _user.password);
	StringProcess::Replace(query, "$name", _user.name);
	StringProcess::Replace(query, "$email", _user.email);
	StringProcess::Replace(query, "$phone_number", _user.phone_number);
	StringProcess::Replace(query, "$avatar", _user.avatar);
	StringProcess::Replace(query, "$address", _user.address);

	return ConnectionManager::QueryOrther(query, _user.id);
}
int UserModel::updateUser(int type, User _user, std::string new_password)
{
	std::string query = R"(UPDATE "user" SET )";

	if (type == 1)
	{
		if (_user.user_name == new_password)
		{
			return 114;
		}

		if (!hasUser(_user, "password"))
		{
			return 109;
		}
		query += R"( password = '$new_password' )";
		query += R"( WHERE user_name = '$user_name' RETURNING id; )";
		StringProcess::Replace(query, "$new_password", new_password);
	}
	if (type == 0)
	{
		if (_user.name != "")		  query += R"( name = '$name',)";
		if (_user.phone_number != "") query += R"( phone_number = '$phone_number',)";
		if (_user.avatar != "")		  query += R"( avatar = decode('$avatar', 'base64'),)";
		if (_user.address != "")	  query += R"( address = '$address',)";
		if (_user.address != "")	  query += R"( email = '$email',)";

		query.pop_back();

		query += R"( WHERE user_name = '$user_name' RETURNING id; )";

		StringProcess::Replace(query, "$password", _user.password);
		StringProcess::Replace(query, "$name", _user.name);
		StringProcess::Replace(query, "$email", _user.email);
		StringProcess::Replace(query, "$phone_number", _user.phone_number);
		StringProcess::Replace(query, "$avatar", _user.avatar);
		StringProcess::Replace(query, "$address", _user.address);
	}

	StringProcess::Replace(query, "$user_name", _user.user_name);

	return ConnectionManager::QueryOrther(query, _user.id);
}
int UserModel::getUser(std::string type,std::vector<User>& _user, std::string auid)
{
	std::string query = 
		R"(SELECT user_name,name,email,phone_number,address,ENCODE(avatar::bytea, 'BASE64'), id 
			FROM "user" 
			WHERE id = '$id' )";
	if (type == "id")
	{
		StringProcess::Replace(query, "$id", _user[0].id);
		StringProcess::Replace(query, "$password", _user[0].password);
	}
	else if(type == "all")
	{
		_user.clear(); 
		query = R"(SELECT * FROM "user")"; 
	}
	else if (type == "one")
	{
		_user.clear();
		query = R"(SELECT * FROM "user" u
					JOIN resourcemanager r ON u.id = r.rscid
					WHERE r.auid = '$auid' AND r.rscname = 'user';
					)";

		StringProcess::Replace(query, "$auid", auid);
	}
	//----------------------------------------------------------------------------------------
	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int  test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string name = poFeature->GetFieldAsString("name");
			std::string user_name = poFeature->GetFieldAsString("user_name");
			std::string email = poFeature->GetFieldAsString("email");
			std::string phone_number = poFeature->GetFieldAsString("phone_number");
			std::string address = poFeature->GetFieldAsString("address");
			std::string avatar = poFeature->GetFieldAsString(5);
			//std::string avatar = poFeature->GetFieldAsString("avatar");
			std::string id = poFeature->GetFieldAsString("id");

			User tmp(user_name, "", name , email, (phone_number == "NULL") ? "" : phone_number, (avatar == "NULL") ? "" : avatar, (phone_number == "NULL") ? "" : address, id);
			if (type == "all")_user.push_back(tmp);
			else _user[0] = tmp; 
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int UserModel::deleteUser(std::string user_name, std::string id )
{
	//if (user_name == "guest" || user_name == "admin")
	//{
	//	//msg = "can not delete this user";
	//	return 201;
	//}
	if (id == "088280a4-f196-464f-8b00-869139d13392" || id == "c22f41e6-9b31-4ba3-880e-9446a79f66e4")
		{
		//msg = "can not delete this user";
		return 201;
	}
	std::string query = 
			R"(DELETE FROM "user" 
				WHERE id = '$id' 
				RETURNING id; )";

	StringProcess::Replace(query, "$id", id);

	return ConnectionManager::QueryOrther(query, id);
}
//-------------------------------------------------------------------------------------------------------------------------
