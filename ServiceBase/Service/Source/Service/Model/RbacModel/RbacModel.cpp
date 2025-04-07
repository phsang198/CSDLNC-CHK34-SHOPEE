#pragma once 
#include "pch.h"
#include "RbacModel.h"
#include "Token/Token.h"
#include "ConnectionPool/ConnectionPool.h"


RbacModel::RbacModel()
{
}

RbacModel::~RbacModel()
{
}

int RbacModel::addRbac(rbac& _rbac)
{
	std::string query =
		R"(INSERT INTO "rbac"(roleid,apiid) 
				  VALUES('$roleid','$apiid')
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$roleid", _rbac.roleid);
	StringProcess::ReplaceAll(query, "$apiid", _rbac.apiid);

	return ConnectionManager::QueryOrther(query, _rbac.id);
}

int RbacModel::getRbac(std::vector<rbac>& lst_rbac)
{
	std::string query = R"(SELECT * FROM "rbac"  )";

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string roleid = poFeature->GetFieldAsString("roleid");
			std::string apiid = poFeature->GetFieldAsString("apiid");
			std::string id = poFeature->GetFieldAsString("id");

			rbac _rbac(roleid, apiid, id);
			lst_rbac.push_back(_rbac); 
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int RbacModel::deleteRbac(const std::string& id)
{
	std::string query =
		R"(DELETE from "rbac"
			WHERE "rbac".id = '$id' 
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$id", id);

	std::string tmpid; 
	return ConnectionManager::QueryOrther(query, tmpid);
}

