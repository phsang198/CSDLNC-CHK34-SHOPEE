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
#include "S3W/S3Wrapper.h"
#include "License/LicenseManager.h"
#include "Poco/Zip/Compress.h"

#include "Token/Token.h"
#include "Validate/Validate.h"

#include "Model/UserModel/UserModel.h"
#include "Model/AuthorizeModel/AuthorizeModel.h"
#include "Model/RoleModel/RoleModel.h"
#include "Model/ApiModel/ApiModel.h"
#include "Model/RbacModel/RbacModel.h"
#include "Model/ResourceModel/ResourceModel.h"

#include "Model/ProviderModel/ProviderModel.h"
#include "Model/ScheduleModel/ScheduleModel.h"

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
	tokenManager m_token;
	std::string productid;
private:
	std::map<std::string, std::map<std::string, std::string>> lstAutho;
public:
	struct context
	{
		std::map<std::string, authorize> listRoles;
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

	void setMiddleware(tokenManager TKManager, std::string _productid);
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
	std::shared_ptr<CLicenseManager>	_pLicenseMngr;
	static CIDNService* m_instance;

	CModel m_Model;
	UserModel m_UserModel;
	AuthorizeModel m_AuthorizeModel;
	RoleModel m_RoleModel;
	ApiModel m_ApiModel;

	RbacModel m_RbacModel;
	ResourceModel m_ResourceModel;
	ProviderModel m_ProviderModel;

	ScheduleModel m_ScheduleModel;

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

	void API_SCHEDULE_GET_METHOD();

	void API_SCHEDULE_POST_METHOD();

	void API_SCHEDULE_PUT_METHOD();

	void API_SCHEDULE_DELETE_METHOD();

	void API_SCHEDULE();

	void API_WEBHOOK();

	void API_SERVICEMESH();

public:
	//-----------------------------
	


public:
	std::shared_ptr<CLicenseManager> GetLicenseManager() { return _pLicenseMngr; }

protected:
	void onDone(const void*, const Poco::Zip::ZipLocalFileHeader& hdr);
	void ServiceWorkerThread(void);
};

