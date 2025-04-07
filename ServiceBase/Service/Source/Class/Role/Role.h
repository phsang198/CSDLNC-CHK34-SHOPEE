#pragma once

class role
{
public:
	role() ;
	role( std::string _name, std::string _id = "");
	~role() {};
public:
	std::string id;
	std::string name;

};

class roleManager
{
public:
	static std::map<std::string, role> lst_role; // first : roleid , second : detail 
	static std::map<std::string, std::string> lst_role_name; // first : role name , second : roleid 
public:
	static void id2name();
};
