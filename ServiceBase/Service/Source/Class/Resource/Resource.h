#pragma once

class resource
{
public:
	resource();
	resource(std::string _auid, std::string _rscname, std::string _rscid,
		std::string _belongname = "", std::string _belongid = "", std::string _id = "");
	~resource() {};
public:
	std::string id;
	std::string auid;
	std::string rscname;
	std::string rscid;
	std::string belongname;
	std::string belongid;

};

class resourceManager
{
public:
	static std::map<std::string, resource> lst_resource; // first : resourceid , second : detail 
	static std::map<std::string, std::string> lst_resource_name; // first : resource name , second : resourceid 
public:
	static void id2name();
};
