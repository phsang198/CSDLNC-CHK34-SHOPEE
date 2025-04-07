#include "pch.h"
#include "ConnectionPool.h"
//-----------------------------------------------------------------------------------------------------------
int ConnectionManager::QueryOrther(std::string query, std::string& Id)
{
	GDALDataset* geDS = CConnectionPool::getConnection();
	if (geDS == NULL)
	{
		return 301;
	}
	OGRLayer* poLayer = geDS->ExecuteSQL(query.c_str(), NULL, "");

	if (poLayer == NULL)
	{
		int errorCode = CPLGetLastErrorNo();
		const char* errorMsg = CPLGetLastErrorMsg();
		std::string errorMessage(errorMsg);

		CConnectionPool::addConnection(geDS);

		if (errorMessage != "")
		{
			//msg = "invalid database connection";
			if (errorMessage == "no connection to the server\n")
			{
				CConnectionPool::reConnect(geDS);
				return 301;
			}
			return 302;
		}
		else
		{
			return 303;
		}
	}
	if (poLayer != NULL)
	{
		if (poLayer->GetFeatureCount() == 0)
		{
			geDS->ReleaseResultSet(poLayer);
			CConnectionPool::addConnection(geDS);
			return 303;
		}
	}

	OGRFeature* poFeature = poLayer->GetNextFeature();
	if (poFeature != NULL)
	{
		Id = poFeature->GetFieldAsString("id");					// co returning
	}

	geDS->ReleaseResultSet(poLayer);
	CConnectionPool::addConnection(geDS);
	return 200;
}
int ConnectionManager::QueryGet(GDALDataset*& geDS, OGRLayer*& poLayer, std::string& query)
{
	geDS = CConnectionPool::getConnection();
	if (geDS == NULL)
	{
		return 301;
	}
	poLayer = geDS->ExecuteSQL(query.c_str(), NULL, "");
	//Sleep(5000); 

	if (poLayer == NULL)
	{
		int errorCode = CPLGetLastErrorNo();
		const char* errorMsg = CPLGetLastErrorMsg();
		std::string errorMessage(errorMsg);

		CConnectionPool::addConnection(geDS);

		if (errorMessage != "")
		{
			//msg = "invalid database connection";
			if (errorMessage == "no connection to the server\n")
			{
				CConnectionPool::reConnect(geDS);
				return 301;
			}
			return 302;
		}
		else
		{
			return 303;
		}
	}
	if (poLayer != NULL)
	{
		if (poLayer->GetFeatureCount() == 0)
		{
			geDS->ReleaseResultSet(poLayer);
			return 303;
		}
	}

	return 200;
}

