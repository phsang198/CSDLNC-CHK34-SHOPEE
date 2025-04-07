#pragma once 
#include "pch.h"
#include "AuthorizeModel.h"
#include "ConnectionPool/ConnectionPool.h"
#include "Authorization/Authorization.h"

AuthorizeModel::AuthorizeModel()
{
}

AuthorizeModel::~AuthorizeModel()
{
}
int AuthorizeModel::getAutho(std::string type, std::string productid, std::string auid , std::vector<authorize>& listAutho)
{
	std::string query = R"( SELECT * FROM "authorization"
							JOIN resourcemanager ON "authorization".userid = resourcemanager.rscid
							WHERE resourcemanager.rscname = 'user' AND "authorization".auid = '$auid' AND 
								  "authorization".productid = '$productid'  )";

	if (type == "all")
		query = R"(SELECT * FROM "authorization" )"; 
	else if ( type == "userid")
		query = R"(SELECT * FROM "authorization" WHERE userid = '$userid')";


	StringProcess::Replace(query, "$userid", auid);
	StringProcess::Replace(query, "$auid", auid);
	StringProcess::Replace(query, "$productid", productid);

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string id = poFeature->GetFieldAsString("id");
			std::string userid = poFeature->GetFieldAsString("userid");
			std::string roleid = poFeature->GetFieldAsString("roleid");
			std::string productid = poFeature->GetFieldAsString("productid");

			authorize tmp(userid, roleid, productid, id);
			listAutho.push_back(tmp);
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
//-------------------------------------------------------------------------------------------------------------------------
int AuthorizeModel::createAutho(authorize& _autho)
{
	std::string query = R"(INSERT INTO "authorization"(userid, roleid , productid) 
								  VALUES('$userid','$roleid','$productid')
						   RETURNING id ;)";

	StringProcess::Replace(query, "$userid", _autho.userid);
	StringProcess::Replace(query, "$roleid", _autho.roleid);
	StringProcess::Replace(query, "$productid", _autho.productid);

	return ConnectionManager::QueryOrther(query, _autho.id);
}
//-------------------------------------------------------------------------------------------------------------------------
int AuthorizeModel::createAuthoFromTmpAutho(std::string& tmp_autho_id)
{
	std::string query = R"(INSERT INTO "authorization" (userid, roleid, productid)
							SELECT userid, roleid, productid
							FROM "tmp_autho"
							WHERE "tmp_autho".id = '$id'
							RETURNING "authorization".userid )";

	StringProcess::Replace(query, "$id", tmp_autho_id);

	return ConnectionManager::QueryOrther(query, tmp_autho_id);
}
//-------------------------------------------------------------------------------------------------------------------------
int AuthorizeModel::updateAutho(authorize _autho, std::string new_autho)
{
	std::string query = "UPDATE \"authorization\" AS a SET info = '$info' ";
	if (new_autho != "")
	{
		query += ", role_id = r2.id ";
	}
	query += '\n';
	query += "FROM \"user\" AS u, \"role\" AS r, \"product\" AS p, \"role\" AS r2 ";
	query += '\n';
	query += "WHERE p.name = '$service'  AND r.role = '$role' AND u.user_name = '$user_name' ";
	query += '\n';
	query += "AND p.id = a.product_id AND r.id = a.role_id AND u.id = a.user_id ";

	//StringProcess::Replace(query, "$user_name", _role.user_name);
	//StringProcess::Replace(query, "$role", _role.roles);
	//StringProcess::Replace(query, "$service", _role.service);
	//StringProcess::Replace(query, "$info", _role.info);

	if (new_autho != "")
	{
		query += "AND r2.role = '$new_role' ";
		StringProcess::Replace(query, "$new_role", new_autho);
	}
	query += '\n';
	query += "RETURNING *";
	std::string id = "";
	return ConnectionManager::QueryOrther(query, _autho.id);
}
int AuthorizeModel::deleteAutho(std::string id )
{
	std::string query = R"(DELETE from "authorization" WHERE id = '$id' RETURNING id)";

	StringProcess::Replace(query, "$id", id);

	return ConnectionManager::QueryOrther(query, id);
}
int AuthorizeModel::processGetAutho(std::string user_name, std::string password, std::string productid, std::vector<authorize>& listAutho)
{
	int error_code = getAutho(user_name, password, productid, listAutho); 
	if (error_code != 200) return error_code; 
	/*bool check = false; 
	for (int i = 0 ; i < listAutho.size() ; i++ )
	{
		authorize* tmp = &listAutho[i];
		error_code = getAllAutho(tmp);
		if (error_code == 200) check = true; 
	}*/
	/*if (check) return 200;
	else return 303; */
	return 200; 
}
int AuthorizeModel::getAllAutho( authorize*& autho)
{
	std::string query = 
		R"(select buildingid 
			from buildingmanager as bm
			where bm.auid = '$auid')";

	StringProcess::Replace(query, "$auid", autho->id);

	//------------------------------------------------------------------------------------------
	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200 )
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string buildingid = poFeature->GetFieldAsString("buildingid");
			//autho->buildingid.push_back(buildingid); 
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
//-------------------------------------------------------------------------------------------------------------------------
int AuthorizeModel::getTmpAutho(std::string type, std::string auid, std::vector<tmp_authorize>& tmp_autho, std::string id)
{
	std::string query = R"( SELECT * FROM tmp_autho
							JOIN resourcemanager ON tmp_autho.buildingid = resourcemanager.belongid
							WHERE resourcemanager.auid = '$auid' AND resourcemanager.belongname = 'building' AND 
									resourcemanager.rscname = 'user' )";

	if (type == "all")
		query = R"(SELECT * FROM tmp_autho )";
	else if ( type == "id")
		query = R"(SELECT * FROM tmp_autho WHERE id = '$id'  )";

	StringProcess::Replace(query, "$auid", auid);
	StringProcess::Replace(query, "$id", id);

	//------------------------------------------------------------------------------------------
	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
			std::string buildingid = poFeature->GetFieldAsString("buildingid");
			std::string productid = poFeature->GetFieldAsString("productid");
			std::string roleid = poFeature->GetFieldAsString("roleid");
			std::string userid = poFeature->GetFieldAsString("userid");
			std::string status = poFeature->GetFieldAsString("status");
			std::string id = poFeature->GetFieldAsString("id");

			tmp_autho.push_back(tmp_authorize(userid, roleid, productid, buildingid, status, id));
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}
int AuthorizeModel::createTmpAutho(tmp_authorize& _autho)
{
	std::string query = R"(INSERT INTO "tmp_autho"(userid, roleid, buildingid , productid) 
								  VALUES('$userid','$roleid','$buildingid','$productid') RETURNING id)";

	StringProcess::Replace(query, "$userid", _autho.userid);
	StringProcess::Replace(query, "$roleid", _autho.roleid);
	if (_autho.buildingid != "")
		StringProcess::Replace(query, "$buildingid", _autho.buildingid);
	else 
		StringProcess::Replace(query, R"('$buildingid')", "NULL");

	StringProcess::Replace(query, "$productid", _autho.productid);

	return ConnectionManager::QueryOrther(query, _autho.id);
}
int AuthorizeModel::updateTmpAutho(std::string id , std::string status)
{
	std::string query = R"(UPDATE "tmp_autho" SET status = '$status' WHERE id = '$id' 
						RETURNING id)";

	StringProcess::Replace(query, "$status", status);
	StringProcess::Replace(query, "$id", id);

	return ConnectionManager::QueryOrther(query, id);
}
int AuthorizeModel::deleteTmpAutho(std::string id, std::string userid)
{
	std::string query = R"(DELETE from "tmp_autho"  WHERE id = '$id' AND status = 'waiting'
						RETURNING id)";

	StringProcess::Replace(query, "$id", id);
	StringProcess::Replace(query, "$userid", userid);

	return ConnectionManager::QueryOrther(query, id);
}