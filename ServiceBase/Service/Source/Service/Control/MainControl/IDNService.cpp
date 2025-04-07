#pragma once
#include "pch.h"
#include "IDNService.h"
#include "ThreadPool/ThreadPool.h"
#include "filereadstream.h"
#include "CConfig/Config.h"

#include "Provider/Provider.h"
#include "ConnectionPool/CrowRequestPool.h"
#include "mesh/mesh.h"

#include "Inc/Task/task_impl.hpp"

CIDNService* CIDNService::m_instance = nullptr;


CIDNService::CIDNService(LPCWSTR pszServiceName,
	DWORD dwArgc,
	LPCWSTR* pszArgv,
	BOOL fCanStop,
	BOOL fCanShutdown,
	BOOL fCanPauseContinue)
	: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
	m_dwArgc = dwArgc;
	m_pszArgv = pszArgv;
	m_instance = this;
	_pLicenseMngr = std::make_shared<CLicenseManager>();

	auto& cors = app.get_middleware<crow::CORSHandler>();
	cors
		.global()
		.headers("X-Custom-Header", "Upgrade-Insecure-Requests")
		.headers("Access-Control-Allow-Headers","Content-Type")
		.headers("Access-Control-Allow-Methods","OPTIONS, GET, POST, PUT, PATCH, DELETE")
		.headers("Accept-Encoding","gzip")
		.headers("Access-Control-Allow-Origin","*")
		.methods("POST"_method, "GET"_method, "OPTIONS"_method, "PUT"_method, "PATCH"_method, "DELETE"_method)
		.prefix("/cors")
		.origin("example.com")
		.prefix("/nocors")
		.ignore();
}


CIDNService::~CIDNService(void)
{
}

void CIDNService::onDone(const void*, const Poco::Zip::ZipLocalFileHeader& hdr)
{
	std::string szName = hdr.getFileName();
	szName = "";
}
void CIDNService::OnStart(DWORD dwArgc, PSTR* pszArgv)
{
	// Log a service start message to the Application log.

	// Queue the main service function for execution in a worker thread.
	WriteEventLogEntry(TEXT("ScheduleService start!"), EVENTLOG_INFORMATION_TYPE);
	Poco::Logger::root().information("OnStart func - stt: ScheduleService start!", __FILE__, __LINE__);

	CThreadPool::QueueUserWorkItem(&CIDNService::ServiceWorkerThread, this);
}
void CIDNService::OnStartDebug(DWORD dwArgc, PSTR* pszArgv)
{
	WriteEventLogEntry(TEXT("ScheduleService debug start!"), EVENTLOG_INFORMATION_TYPE);
	Poco::Logger::root().information("OnStartDebug func - stt: ScheduleService debug start!", __FILE__, __LINE__);

	handleRoutes();
	Start();
}

void CIDNService::ServiceWorkerThread(void)
{
	WriteEventLogEntry(TEXT("In Queue Star"), EVENTLOG_INFORMATION_TYPE);
	Poco::Logger::root().information("ServiceWorkerThread func - stt: In Queue Star!", __FILE__, __LINE__);

	handleRoutes();
	Start();
}

void CIDNService::OnStop()
{
	WriteEventLogEntry(TEXT("ScheduleService stop!"), EVENTLOG_INFORMATION_TYPE);
	Poco::Logger::root().information("OnStop func - stt: ScheduleService stop!", __FILE__, __LINE__);


#ifndef _DEBUG
	if (_pLicenseMngr)
		_pLicenseMngr->Stop();
#endif
	m_Model.Destroy();
	app.stop();
}

BOOL CIDNService::ConsoleCtrlHandler(DWORD ctrlType)
{
	switch (ctrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
		s_service->Stop();
		return TRUE;
	default:
		return FALSE;
	}
}

//-------------START SERVIECE-------------//
void CIDNService::Start()
{
	int servicePort, concurrency, maxQueue, timeOut;

	std::string dbName, dbHost, dbUser, dbPassword;
	int dbPort;

	Config::Config();
	Config::loadTokenConfig(product::host);

	Config::loadDBConfig( dbName, dbHost, dbPort, dbUser, dbPassword);
	Config::loadServiceConfig(servicePort, concurrency, maxQueue, timeOut);

	Config::loadErrorConfig();

	CrowRequestPool::createPool(maxQueue, timeOut);
	
#ifndef _DEBUG
	if (_pLicenseMngr)
		_pLicenseMngr->Start();
#endif

	
	//-----------------------------------------------------------------// connect to server token 
	if (!product::getProduct("WorkFlowService"))
	{
		Poco::Logger::root().information("Can not connect to TOKEN server!", __FILE__, __LINE__);
		WriteEventLogEntry(TEXT("Can not connect to TOKEN server!"), EVENTLOG_INFORMATION_TYPE);
		exit(3);
		return;
	}
	else
	{
		Poco::Logger::root().information("Connected to TOKEN server!", __FILE__, __LINE__);
	}
	//-----------------------------------------------------------------// connect to server DB 
	if (!m_Model.Connect(dbName, dbHost, dbPort, dbUser, dbPassword)) 
	{
		Poco::Logger::root().information("Start func - stt: Cannot connect to db , error configFile!", __FILE__, __LINE__);
		WriteEventLogEntry(TEXT("Cannot connect to db!"), EVENTLOG_INFORMATION_TYPE);
		return;
	}
	else
	{
		/*std::string content;
		if (Config::readFileConfig(SupportFunc::getExePath(RUN_PROJECT) + FILE_SCHEMA_CONFIG_PATH, content))
			m_Model.initSchema(content);
			*/

		Poco::Logger::root().information("Start func - stt: Connected to DB!", __FILE__, __LINE__);
	}
	//-----------------------------------------------------------------// connect to server minio 
	
	{
		Config::loadSMeshConfig(mesh::host, mesh::config);
		while (1)
		{
			std::cout << "Registering mesh ...." << std::endl; 
			if (mesh::registerMesh())
			{
				Poco::Logger::root().information("Registered mesh!", __FILE__, __LINE__);
				break;
			}
			else
			{
				Poco::Logger::root().information("Can not register mesh!", __FILE__, __LINE__);
				Sleep(20000);
			}
		}

		std::vector<rbac> tmp_rbac;
		std::vector<CApi> tmp_api;
		std::vector<role> tmp_role;

		m_RbacModel.getRbac(tmp_rbac);
		m_RoleModel.getRole(tmp_role);
		m_ApiModel.getApi(tmp_api);

		SupportFunc::vector2map(tmp_api, apiManager::lst_api);
		SupportFunc::vector2map(tmp_role, roleManager::lst_role);
		roleManager::id2name(); 
		rbacManager::vector2map(tmp_rbac);

		m_ScheduleModel.getSchedule();
		m_ScheduleModel.getExcuteSchedule();
		Poco::Logger::root().information("Start func - stt: Cache schedule!", __FILE__, __LINE__);

		VBD::Task::Initialize();
		schedule::manager::timer.run(); 

	}


	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
	int concurrencyused = (std::max)((double)concurrency, (double)std::thread::hardware_concurrency());

	//using namespace crow::security_middleware;
	//using Type = Sources::Type;

	///*app.use(new SecurityMiddleware())
	//	.setXFrameOptions(XFrameOptions::DENY)
	//	.setXSSProtection(XSSProtection::BLOCK)
	//	.setNoSniff()
	//	.setStrictTransportSecurity(31536000)
	//	.setAccessControlAllowOrigin("http://mysite.com")
	//	.setContentSecurityPolicy(Sources().trust(Type::DEFAULT, Sources::kSelf)
	//		.trust(Type::FONT, "https://themes.googleusercontent.com"))
	//	.setCrossDomainMetaPolicy(PermittedCrossDomainMetaPolicy::MASTER_ONLY);*/


	app.get_middleware<SecurityMiddleware>().setMiddleware(product::m_TokenManager, product::id);
	app.server_name("Schedule");
	app.port(servicePort)
		.timeout(1)
		.concurrency(concurrencyused)
		.run();


	//auto& cors = app.get_middleware<crow::CORSHandler>();

	//// clang-format off
	//cors
	//	.global()
	//	.headers("X-Custom-Header", "Upgrade-Insecure-Requests")
	//	.methods("POST"_method, "GET"_method)
	//	.prefix("/cors")
	//	.origin("example.com")
	//	.prefix("/nocors")
	//	.ignore();

}
void CIDNService::Destroy()
{
	m_Model.Destroy();
	VBD::Task::OnDestroy();

}

//=======================================================================================================================
void CIDNService::handleRoutes()
{
	API_SCHEDULE();
	API_WEBHOOK(); 
	API_SERVICEMESH(); 
}
