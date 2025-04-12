// ScheduleService.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "ServiceInstaller/ServiceInstaller.h"
#include "ServiceBase/ServiceBase.h"
#include "Control/MainControl/IDNService.h"
#include <iostream>

// Poco.
#include "Poco/UnicodeConverter.h"

// 
// Settings of the service
// 



// Displayed name of the service
#define SERVICE_DISPLAY_NAME     TEXT("Service(C++)")
// Service start options.
#define SERVICE_START_TYPE       SERVICE_AUTO_START//SERVICE_DEMAND_START
// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     TEXT("")
// The name of the account under which the service should run
#define SERVICE_ACCOUNT          TEXT("NT AUTHORITY\\LocalService")
// The password to the service account name
#define SERVICE_PASSWORD         NULL


int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring szCurDir(argv[0]), szModuleName(argv[0]);
	int nIndex = static_cast<int>(szCurDir.find_last_of(L"\\"));
	szCurDir = szCurDir.substr(0, nIndex + 1);
	szModuleName = szModuleName.substr(nIndex + 1, szModuleName.find_last_of(L".exe") - nIndex - 4);
	CIDNService service(szModuleName.c_str(), argc, const_cast<LPCWSTR*>(argv));

#ifdef _DEBUG
	CServiceBase::RunDebug(service);
#else

	if ((argc > 1) && ((*argv[1] == '-') || (*argv[1] == '/')))
	{
		if (lstrcmpi(TEXT("install"), argv[1] + 1) == 0)
		{
			/*Install the service when the command is
			"-install\"or "/install".*/
			InstallService(
				service.GetName(),               // Name of service
				SERVICE_DISPLAY_NAME,       // Name to display
				SERVICE_START_TYPE,         // Service start type
				SERVICE_DEPENDENCIES,       // Dependencies
				SERVICE_ACCOUNT,            // Service running account
				SERVICE_PASSWORD            // Password of the account
			);
		}
		else if (lstrcmpi(TEXT("remove"), argv[1] + 1) == 0)
		{
			/*Uninstall the service when the command is
			"-remove\"or "/remove".*/
			UninstallService(service.GetName());
		}
		else if (lstrcmpi(TEXT("debug"), argv[1] + 1) == 0)
		{
			CServiceBase::RunDebug(service);
		}
	}
	else
	{
		if (!CIDNService::Run(service))
		{
			if (ERROR_FAILED_SERVICE_CONTROLLER_CONNECT == GetLastError())
				printf("Service failed to run err %d\n", GetLastError());
		}
	}
#endif

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
