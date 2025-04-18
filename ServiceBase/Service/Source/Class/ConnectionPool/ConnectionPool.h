#pragma once
#include "ogrsf_frmts.h"
class CConnectionPool
{
public:
	~CConnectionPool();
	static void createPool(std::string dbName, std::string dbHost, int dbPort, std::string dbUser, std::string dbPassword, double timeout);
	static void reConnect(GDALDataset*& connection);
	static void resetPool();
	static void addConnection(GDALDataset* connection);
	static GDALDataset* getConnection();
	static BOOL isEmpty();
	//------------------------------------------------------------------------------------------------

public:
	static std::queue<GDALDataset*> cData;
	static std::vector<GDALDataset*> m_pData;

	static int size;
	static double m_timeout;

	static std::string db_path;
};

class ConnectionManager
{
public:
	static int QueryOrther(std::string query, std::string& Id, std::string rRes = "id");
	static int QueryGet(GDALDataset*& geDS, OGRLayer*& poLayer, std::string& query);
};