#pragma once 
#include "pch.h"
#include "RoleModel.h"
#include "Role/Role.h"
#include "Token/Token.h"
#include "ConnectionPool/ConnectionPool.h"


RoleModel::RoleModel()
{
}

RoleModel::~RoleModel()
{
}

int RoleModel::addRole(role& _role)
{
	std::string query =
		R"(INSERT INTO "role"(name) 
			VALUES ('$name')
			ON CONFLICT (name)
			DO 
				UPDATE SET name = '$name'
				WHERE "role".name = '$name' 
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$rank", _role.id);

	return ConnectionManager::QueryOrther(query, _role.id);
}

int RoleModel::getRole(std::vector<role>& lst_role)
{
	std::string query = R"(SELECT * FROM "role"  )";

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string name = poFeature->GetFieldAsString("name");
			std::string id = poFeature->GetFieldAsString("id");

			role _role(name, id);
			lst_role.push_back(_role); 
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int RoleModel::deleteRole(const std::string& id)
{
	std::string query =
		R"(DELETE from "role"
			WHERE "role".id = '$id' 
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$id", id);

	std::string tmpid; 
	return ConnectionManager::QueryOrther(query, tmpid);
}

