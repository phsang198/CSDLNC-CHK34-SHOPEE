#include "pch.h"
#include "CrowRequestPool.h"
#include <condition_variable>
#include <thread>
#include <chrono>
#include <atomic>
using namespace std::chrono_literals;
//Mutex pool 
std::condition_variable cvCrow;
std::mutex cv_mCrow;

int CrowRequestPool::size = 12;
int CrowRequestPool::m_timeout = 1000;
std::queue<int*> CrowRequestPool::cData;


//CrowRequestPool::CrowRequestPool()
//{
//	//Exit connection 
//	for (int i = 0; i < size; i++)
//	{
//		int* k = new int;
//		cData.push(k);
//	}
//}
CrowRequestPool::~CrowRequestPool()
{
	while (!cData.empty())
	{
		int* element = cData.front();
		delete element;
		cData.pop();
	}
}

void CrowRequestPool::createPool(int _size, int timeout)
{
	size = _size;
	m_timeout = timeout;

	for (int i = 0; i < size; i++)
	{
		int* k = new int;
		cData.push(k);
	}
}

void CrowRequestPool::addConnection(int* connection, std::string request_str)
{
	std::lock_guard<std::mutex> lk(cv_mCrow);

	Poco::Logger::root().information(request_str, __FILE__, __LINE__);

	if (cData.size() == size) return;
	if (connection != NULL)
		cData.push(connection);
	cvCrow.notify_all();
}

int* CrowRequestPool::getConnection()
{
	std::unique_lock<std::mutex> lk(cv_mCrow);

	if (cvCrow.wait_for(lk, m_timeout * 1ms, [] {return cData.size() != 0;  }))
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