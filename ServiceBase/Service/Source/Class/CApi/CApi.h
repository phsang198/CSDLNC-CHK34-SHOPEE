#pragma once

class CApi
{
public:
	CApi() ;
	CApi( std::string _root, std::string _version, std::string _resource, std::string _sub_resource, 
		  std::string _method, std::string _attribute ,std::string _id = "");
	~CApi() {};
public:
	std::string id;
	std::string root;
	std::string version;
	std::string resource;
	std::string sub_resource;
	std::string method;
	std::string attribute;
};

class apiManager
{
public:
	static std::map<std::string, CApi> lst_api;
};