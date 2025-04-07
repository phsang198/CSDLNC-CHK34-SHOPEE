#pragma once 
#include "pch.h"
#include "ResourceModel.h"
#include "Token/Token.h"
#include "ConnectionPool/ConnectionPool.h"


ResourceModel::ResourceModel()
{
}

ResourceModel::~ResourceModel()
{
}

int ResourceModel::addResource(resource& _Resource)
{
	std::string query =
		R"(INSERT INTO "resourcemanager"(auid,rscid,rscname,belongname,belongid) 
			VALUES ('$auid','$rscid','$rscname','$belongname','$belongid')
			RETURNING id )";

	StringProcess::ReplaceAll(query, "$auid", _Resource.auid);
	StringProcess::ReplaceAll(query, "$rscid", _Resource.rscid);
	StringProcess::ReplaceAll(query, "$rscname", _Resource.rscname);
	if (_Resource.belongid != "")
		StringProcess::ReplaceAll(query, "$belongname", _Resource.belongname);
	else
		StringProcess::ReplaceAll(query, R"('$belongname')", "NULL");
	if (_Resource.belongid != "")
		StringProcess::ReplaceAll(query, "$belongid", _Resource.belongid);
	else
		StringProcess::ReplaceAll(query, R"('$belongid')", "NULL");

	return ConnectionManager::QueryOrther(query, _Resource.id);
}

RESOURCETYPE ResourceModel::createSchedule(OGRFeature* poFeature)
{
	std::string scdname = poFeature->GetFieldAsString("scdname");
	std::string wid = poFeature->GetFieldAsString("wid");
	std::string scdid = poFeature->GetFieldAsString("id");
	std::string inss = poFeature->GetFieldAsString("instance");
	int itimeout = poFeature->GetFieldAsInteger("itimeout");
	int wtimeout = poFeature->GetFieldAsInteger("wtimeout");

	return std::make_shared<schedule::registration>(scdname, wid, itimeout, wtimeout, inss, scdid);
}
RESOURCETYPE ResourceModel::createExcuteSchedule(OGRFeature* poFeature)
{
	std::string scdid = poFeature->GetFieldAsString("id");
	std::string config = poFeature->GetFieldAsString("config");
	std::string created = poFeature->GetFieldAsString("created");

	auto tmp = std::make_shared<schedule::registration>("", "", config, "", ToEpochTime(), scdid); 
	tmp->created = created; 
	return tmp; 
}
int ResourceModel::getResource(std::string rscname, std::string auid, std::vector<RESOURCETYPE>& lst_Resource)
{
	std::string query = R"(SELECT * FROM "resourcemanager" 
						   WHERE  rscname = '$rscname' AND auid = '$auid' )";

	if (rscname != "resource")
		query = R"(SELECT * 
					FROM "$rscname"
					WHERE id IN (
						SELECT rscid
						FROM resourcemanager
						WHERE auid = '$auid' AND rscname = '$rscname'
					)
				)";
	if ( auid == "60612643-cb8c-4fc6-acd6-3f3019faf451") 
		query = R"(SELECT * FROM "$rscname" )";

	StringProcess::ReplaceAll(query, "$auid", auid);
	StringProcess::ReplaceAll(query, "$rscname", rscname);

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			if (rscname == "schedule")
			{
				lst_Resource.push_back(createSchedule(poFeature));
			}
			else if (rscname == "excute_schedule")
			{
				lst_Resource.push_back(createExcuteSchedule(poFeature));
			}
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int ResourceModel::deleteResource(const std::string rscname , const std::string& auid, const std::string& id)
{
	std::string query =
		R"(DELETE from "Resource"
			WHERE "Resource".id = '$id' 
			RETURNING id )";

	if (rscname != "resource")
	{
		query = R"(DELETE FROM "$rscname"
					WHERE id IN (
						SELECT rscid
						FROM resourcemanager
						WHERE auid = '$auid' AND rscname = '$rscname'
					) 
				)";
		if (id != "") query += "AND id = '$id'"; 
		query += " RETURNING id ";
	}
	StringProcess::ReplaceAll(query, "$id", id);
	StringProcess::ReplaceAll(query, "$auid", auid);
	StringProcess::ReplaceAll(query, "$rscname", rscname);

	std::string tmpid;
	return ConnectionManager::QueryOrther(query, tmpid);
}

int ResourceModel::Query(const std::string& id, const std::string& geos, const std::string& tablename)
{
	std::string query =
		R"(
			CREATE TABLE IF NOT EXISTS public."geom_data"
			(
				id uuid NOT NULL DEFAULT uuid_generate_v4(),
				geom GEOMETRY 
			))";

	std::string tmpid;
	ConnectionManager::QueryOrther(query, tmpid);

	query = R"(INSERT INTO geom_data (id, geom) 
			VALUES ('$id',ST_GeomFromWKB('$wkb', 4326)) )";
	StringProcess::ReplaceAll(query, "$id", id);
	StringProcess::ReplaceAll(query, "$wkb", geos);

	return ConnectionManager::QueryOrther(query, tmpid);

}

