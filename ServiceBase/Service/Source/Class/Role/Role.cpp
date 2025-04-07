#include "pch.h"
#include "Role.h"

std::map<std::string,role> roleManager::lst_role;
std::map<std::string, std::string> roleManager::lst_role_name;

role::role()
{

}
role::role(std::string _name,  std::string _id )
{
	name = _name;
	id = _id;
}

void roleManager::id2name()
{
	for (auto x : lst_role)
	{
		lst_role_name[x.second.name] = x.first; 
	}
}