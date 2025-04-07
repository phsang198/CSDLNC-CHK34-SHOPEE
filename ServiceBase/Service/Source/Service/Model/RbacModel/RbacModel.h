#pragma once

#include "Rbac/Rbac.h"

class RbacModel
{
public:
	RbacModel();
	~RbacModel();
public:
	int addRbac(rbac& _rbac);
	int getRbac(std::vector<rbac>& lst_rbac);
	int deleteRbac(const std::string& id);
};