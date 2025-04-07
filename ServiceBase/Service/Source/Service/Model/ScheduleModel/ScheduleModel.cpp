#pragma once 
#include "pch.h"
#include "ScheduleModel.h"
#include "Token/Token.h"
#include "ConnectionPool/ConnectionPool.h"


ScheduleModel::ScheduleModel()
{
}

ScheduleModel::~ScheduleModel()
{
}

int ScheduleModel::addExcuteSchedule(std::string name, std::string config , std::string& id)
{
	std::string query =
		R"(INSERT INTO "excute_schedule"(scdname,config) 
			VALUES ('$scdname',$$#config$$)
			RETURNING id )";

	StringProcess::ReplaceAll(query, "$scdname", name);
	StringProcess::ReplaceAll(query, "#config", config);

	return ConnectionManager::QueryOrther(query, id);
}

int ScheduleModel::addSchedule(std::shared_ptr<schedule::registration>& reg)
{
	std::string query =
		R"(INSERT INTO "schedule"(scdname,wid,instance,itimeout,wtimeout) 
			VALUES ('$scdname','$wid','$instance',$itimeout,$wtimeout)
			RETURNING id )";

	StringProcess::ReplaceAll(query, "$scdname", reg->scdname);
	StringProcess::ReplaceAll(query, "$wid", reg->wid);
	StringProcess::ReplaceAll(query, "$instance", reg->inss);
	StringProcess::ReplaceAll(query, "$itimeout", std::to_string(reg->itimeout));
	StringProcess::ReplaceAll(query, "$wtimeout", std::to_string(reg->wtimeout));

	return ConnectionManager::QueryOrther(query, reg->scdid);
}

int ScheduleModel::updateSchedule(std::shared_ptr<schedule::registration> reg, int type)
{
	std::string query =
		R"(UPDATE "schedule" SET instance = '$instance'
			WHERE wid = '$wid'
			RETURNING id )";
	if (type == 1)
	{
		query = R"(UPDATE "schedule" SET instance = '$instance'
			WHERE id = '$schid'
			RETURNING id )";
	}
	StringProcess::ReplaceAll(query, "$scdname", reg->scdname);
	StringProcess::ReplaceAll(query, "$schid", reg->scdid);
	StringProcess::ReplaceAll(query, "$wid", reg->wid);
	StringProcess::ReplaceAll(query, "$instance", reg->inss);
	StringProcess::ReplaceAll(query, "$itimeout", std::to_string(reg->itimeout));
	StringProcess::ReplaceAll(query, "$wtimeout", std::to_string(reg->wtimeout));

	return ConnectionManager::QueryOrther(query, reg->scdid);
}
int ScheduleModel::updateExcuteSchedule(std::string name, std::string config, std::string id)
{
	std::string query =
		R"(UPDATE "excute_schedule" SET scdname = '$name', config = $$#config$$
			WHERE id = '$id'
			RETURNING id )";

	StringProcess::ReplaceAll(query, "$scdname", name);
	StringProcess::ReplaceAll(query, "#config", config);
	StringProcess::ReplaceAll(query, "$id", id);

	return ConnectionManager::QueryOrther(query, id);
}

int ScheduleModel::getExcuteSchedule()
{
	std::string query = R"(SELECT * FROM "excute_schedule" )";

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string name = poFeature->GetFieldAsString("scdname");
			std::string scdid = poFeature->GetFieldAsString("id");
			std::string config = poFeature->GetFieldAsString("config");

			std::shared_ptr<schedule::registration> reg = schedule::registration::parserExcuteSchedule(config) ;
			if (reg != nullptr)
			{
				reg->scdid = scdid;
				schedule::manager::initFrequent(reg);
			}
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int ScheduleModel::getSchedule()
{
	std::string query = R"(SELECT * FROM "schedule" )";

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::shared_ptr<schedule::registration> tmp = std::make_shared<schedule::registration>();

			tmp->scdname = poFeature->GetFieldAsString("scdname");
			tmp->wid = poFeature->GetFieldAsString("wid");
			tmp->scdid = poFeature->GetFieldAsString("id");

			tmp->inss = poFeature->GetFieldAsString("instance");

			tmp->itimeout = poFeature->GetFieldAsInteger64("itimeout");
			tmp->wtimeout = poFeature->GetFieldAsInteger64("wtimeout");

			tmp->parse_ins(); 

			schedule::manager::add(std::move(tmp));

		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int ScheduleModel::deleteSchedule(const std::string& id)
{
	std::string query =
		R"(DELETE from "schedule"
			WHERE "schedule".id = '$id' 
			RETURNING id )";
	if (id == "")
	{
		query =
			R"(DELETE from "schedule"
			RETURNING id )";
	}
	StringProcess::ReplaceAll(query, "$id", id);

	std::string tmpid;
	return ConnectionManager::QueryOrther(query, tmpid);
}
int ScheduleModel::deleteExcuteSchedule(const std::string& id)
{
	std::string query =
		R"(DELETE from "excute_schedule"
			WHERE "excute_schedule".id = '$id' 
			RETURNING id )";
	if (id == "")
	{
		query =
			R"(DELETE from "excute_schedule"
			RETURNING id )";
	}
	StringProcess::ReplaceAll(query, "$id", id);

	std::string tmpid;
	return ConnectionManager::QueryOrther(query, tmpid);
}

