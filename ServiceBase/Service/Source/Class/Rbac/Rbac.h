#pragma once

class rbac
{
public:
	rbac() ;
	rbac( std::string _roleid, std::string _apiid, std::string _id = "");
	~rbac() {};
public:
	std::string id;
	std::string roleid;
	std::string apiid;
};

class rbacManager
{
public:
	static std::map<std::string, std::map<std::string, BOOL>> lst_rbac; // first : roleid , second : api URL 
public: 
	static void vector2map(std::vector<rbac> vetor_rbac);
};