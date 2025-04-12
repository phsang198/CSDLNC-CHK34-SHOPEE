/****************************** Module Header ******************************\
* Module Name:  SampleService.h
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
*
* Provides a sample service class that derives from the service base class -
* CServiceBase. The sample service logs the service start and stop
* information to the Application event log, and shows how to run the main
* function of the service in a thread pool worker thread.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS\"WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once
#include "ServiceBase/ServiceBase.h"
#include "Model/CModel/Model.h"
#include "Poco/Zip/Compress.h"

#include "Validate/Validate.h"

#include "Model/UserModel/UserModel.h"
#include "Model/ResourceModel/ResourceModel.h"

#include "Model/ProviderModel/ProviderModel.h"

class Poco::Zip::ZipLocalFileHeader;

using rUrl = validate::url; 
using rBody = validate::body; 

#define uGetVal			rUrl::getValue 
#define uParam			req.url_params
#define bGetVal			rBody::getValue 
#define bMultipart		rBody::checkMultiPart

class SecurityLogHandler : public crow::ILogHandler {
public:
	void log(std::string /*message*/, crow::LogLevel /*level*/) override {
		//            cerr << "ExampleLogHandler -> " << message;
	}
};

class SecurityMiddleware
{
public:
	std::string message;
	std::string productid;
private:
	std::map<std::string, std::map<std::string, std::string>> lstAutho;
public:
	struct context
	{
		std::string strRoles;
		std::string user_name;
		std::string auid; 
		std::string roleid; 
		std::string userid; 

		std::string token; 

		int* save;
	};
public:
	SecurityMiddleware();

	void setMiddleware();
	void before_handle(crow::request& req, crow::response& res, context& ctx);
	void after_handle(crow::request& req, crow::response& res, context& ctx);
};

class CIDNService : public CServiceBase
{
private:
	int			m_concurrency;
	DWORD		m_dwArgc;
	LPCWSTR*	m_pszArgv;

	//crow::SimpleApp app;
	crow::App<crow::CORSHandler,SecurityMiddleware> app;
	static CIDNService* m_instance;

	CModel m_Model;
	UserModel m_UserModel;
	ResourceModel m_ResourceModel;
	ProviderModel m_ProviderModel;


public:
	CIDNService(LPCWSTR pszServiceName,
		DWORD dwArgc,
		LPCWSTR* pszArgv,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);
	virtual ~CIDNService(void);


public:
	virtual void OnStart(DWORD dwArgc, PSTR* pszArgv);
	virtual void OnStartDebug(DWORD dwArgc, PSTR* pszArgv);
	virtual void OnStop();
	static BOOL __stdcall ConsoleCtrlHandler(DWORD ctrlType);

	void Start();

	void handleRoutes();
	
	void Destroy();


public:

	nlohmann::json GenerateSchedule2JSON(const std::vector<RESOURCETYPE>& reg);

	void API();

public:
	//-----------------------------
	



protected:
	void onDone(const void*, const Poco::Zip::ZipLocalFileHeader& hdr);
	void ServiceWorkerThread(void);
};

