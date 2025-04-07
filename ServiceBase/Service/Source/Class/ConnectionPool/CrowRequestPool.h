#pragma once

class CrowRequestPool
{
public:
	//CrowRequestPool();
	~CrowRequestPool();
	static void createPool(int _size, int timeout);
	static void addConnection(int* connection, std::string request_str);
	static int* getConnection();

public:

	static std::queue<int*> cData;

	static int size;
	static int m_timeout;
};