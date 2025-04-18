#include "pch.h"
#include "ConnectionPool.h"
#include <condition_variable>
#include <thread>
#include <chrono>
#include <atomic>
using namespace std::chrono_literals;
//Mutex pool 
std::condition_variable cv;
std::mutex cv_m;

int CConnectionPool::size = 0;
double CConnectionPool::m_timeout = 0;
std::string CConnectionPool::db_path;

std::queue<GDALDataset*> CConnectionPool::cData;
std::vector<GDALDataset*> CConnectionPool::m_pData;

CConnectionPool::~CConnectionPool()
{
	//Exit connection 
	for (int i = 0; i < m_pData.size(); ++i)
		GDALClose(m_pData.at(i));
	std::queue<GDALDataset*> empty;
	std::swap(cData, empty);
}

void CConnectionPool::createPool(std::string dbName, std::string dbHost, int dbPort, std::string dbUser, std::string dbPassword, double timeout)
{
	GDALAllRegister();
	for (int i = 0; i < size; ++i)
	{
		GDALDataset* geDS;
		std::string path = "PG:dbname=$dbName host=$dbHost port=$dbPort user=$dbUser password =$dbPassword";
		StringProcess::Replace(path, "$dbName", dbName);
		StringProcess::Replace(path, "$dbHost", dbHost);
		StringProcess::Replace(path, "$dbPort", std::to_string(dbPort));
		StringProcess::Replace(path, "$dbUser", dbUser);
		StringProcess::Replace(path, "$dbPassword", dbPassword);
		geDS = (GDALDataset*)(GDALDataset::Open(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL));
		if (geDS)
		{
			CPLSetConfigOption("GDAL_HTTP_TIMEOUT", "5");
			CPLSetConfigOption("PGCLIENTENCODING", "UTF8 timeout=3");
			cData.push(geDS);
			m_pData.push_back(geDS);
		}
		db_path = path;
	}
	m_timeout = timeout;
}

void CConnectionPool::reConnect(GDALDataset*& connection)
{
	std::unique_lock<std::mutex> lk(cv_m);

	GDALClose(connection);

	connection = (GDALDataset*)(GDALDataset::Open(db_path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL));
	if (connection)
	{
		CPLSetConfigOption("GDAL_HTTP_TIMEOUT", "5");
		CPLSetConfigOption("PGCLIENTENCODING", "UTF8 timeout=3");
		cData.push(connection);
		m_pData.push_back(connection);
	}
}

void CConnectionPool::resetPool()
{
	std::lock_guard<std::mutex> lk(cv_m);
	if (cData.size() != 0) return;
	for (int i = 0; i < m_pData.size(); ++i) cData.push(m_pData.at(i));
	cv.notify_all();
}

void CConnectionPool::addConnection(GDALDataset* connection)
{
	std::lock_guard<std::mutex> lk(cv_m);
	if (cData.size() == size) return;
	if (connection != NULL)
		cData.push(connection);
	cv.notify_all();
}

GDALDataset* CConnectionPool::getConnection()
{
	std::unique_lock<std::mutex> lk(cv_m);
	if (cv.wait_for(lk, m_timeout * 1ms, [] {return cData.size() != 0;  }))
	{
		auto geDS = cData.front();
		cData.pop();
		return geDS;
	}
	else
	{
		return NULL;
	}
}

BOOL CConnectionPool::isEmpty()
{
	return cData.size() == 0;
}
