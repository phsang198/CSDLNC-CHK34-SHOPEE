#pragma once
#include <string>
class mesh
{
public:
	static std::string host;
	static std::string thishost;
	static std::string config;
	static std::string created;
public:
	static std::string getISO8601Timestamp();
	static BOOL registerMesh();
};