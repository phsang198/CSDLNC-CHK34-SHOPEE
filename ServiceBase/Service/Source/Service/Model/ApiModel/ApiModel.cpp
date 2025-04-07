#pragma once 
#include "pch.h"
#include "ApiModel.h"
#include "Token/Token.h"
#include "ConnectionPool/ConnectionPool.h"

ApiModel::ApiModel()
{
}

ApiModel::~ApiModel()
{
}

int ApiModel::addApi(CApi& _api)
{
	std::string query =
		R"(INSERT INTO "api"(root,version,resource,sub_resource,method,attribute) 
			VALUES ('$root','$version','$resource','$sub_resource','$method','$attribute')
			ON CONFLICT (root,version, resource, sub_resource, method ) 
			DO NOTHING
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$root", _api.root);
	StringProcess::ReplaceAll(query, "$version", _api.version);
	StringProcess::ReplaceAll(query, "$resource", _api.resource);
	StringProcess::ReplaceAll(query, "$sub_resource", _api.sub_resource);
	StringProcess::ReplaceAll(query, "$method", _api.method);
	StringProcess::ReplaceAll(query, "$attribute", _api.attribute);

	return ConnectionManager::QueryOrther(query, _api.id);
}


int ApiModel::updateApi(CApi _api)
{
	std::string query =
		R"(UPDATE "api" SET root='$root',version='$version',resource='$resource',
                            sub_resource='$sub_resource',method='$method',attribute='$attribute'
			WHERE id = '$id'
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$root", _api.root);
	StringProcess::ReplaceAll(query, "$version", _api.version);
	StringProcess::ReplaceAll(query, "$resource", _api.resource);
	StringProcess::ReplaceAll(query, "$sub_resource", _api.sub_resource);
	StringProcess::ReplaceAll(query, "$method", _api.method);
	StringProcess::ReplaceAll(query, "$attribute", _api.attribute);
	StringProcess::ReplaceAll(query, "$id", _api.id);

	return ConnectionManager::QueryOrther(query, _api.id);
}

int ApiModel::getApi(std::vector<CApi>& lst_api)
{
	std::string query = R"(SELECT * FROM "api"  )";

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string id = poFeature->GetFieldAsString("id");
			std::string root = poFeature->GetFieldAsString("root");
			std::string version = poFeature->GetFieldAsString("version");
			std::string resource = poFeature->GetFieldAsString("resource");
			std::string sub_resource = poFeature->GetFieldAsString("sub_resource");
			std::string method = poFeature->GetFieldAsString("method");
			std::string attribute = poFeature->GetFieldAsString("attribute");

			CApi _api(root, version, resource, sub_resource, method, attribute,id);
			lst_api.push_back(_api); 
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int ApiModel::deleteApi(const std::string& id)
{
	std::string query =
		R"(DELETE from "api"
			WHERE "api".id = '$id' 
			RETURNING id; )";

	StringProcess::ReplaceAll(query, "$id", id);

	std::string tmpid; 
	return ConnectionManager::QueryOrther(query, tmpid);
}

