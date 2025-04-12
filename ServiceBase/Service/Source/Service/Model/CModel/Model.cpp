#pragma once 
#include "pch.h"
#include "Model.h"


CModel::CModel()
{
	//Mmtx.lock(); 
		//Init Cache 

}

CModel::~CModel()
{
	//Mmtx.unlock();
	//if (m_BuildingCache != NULL)
	//{
	//	delete m_BuildingCache;
	//	m_BuildingCache = NULL;
	//}
}
BOOL CModel::initSchema(std::string content)
{
	std::string id;
	return ConnectionManager::QueryOrther(content, id);
}

BOOL CModel::Connect(std::string dbName, std::string dbHost, int dbPort, std::string dbUser, std::string dbPassword)
{
	CConnectionPool::size = 30;
	CConnectionPool::createPool(dbName, dbHost, dbPort, dbUser, dbPassword, 100);
	return !CConnectionPool::isEmpty();
}

void CModel::Destroy()
{
}
