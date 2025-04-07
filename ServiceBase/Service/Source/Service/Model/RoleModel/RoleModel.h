#pragma once

class role; 
class RoleModel
{
public:
	RoleModel();
	~RoleModel();
public:
	int addRole(role& _role);
	int getRole(std::vector<role>& lst_role);
	int deleteRole(const std::string& id);
};