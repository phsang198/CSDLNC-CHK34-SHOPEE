#pragma once

class authorize
{
public:
	authorize() {};
	authorize(std::string _userid, std::string _roleid, std::string _productid, std::string _id = "");
	~authorize() {};
public:
	std::string id;
	std::string userid;
	std::string roleid;
	std::string productid;

};

class tmp_authorize
{
public:
	tmp_authorize() {};
	tmp_authorize(std::string _userid, std::string _roleid, std::string _productid, std::string _buildingid,std::string _status = "", std::string _id = "");
	~tmp_authorize() {};
public:
	std::string id;
	std::string userid;
	std::string roleid;
	std::string productid;
	std::string buildingid = "";
	std::string status; 
};
