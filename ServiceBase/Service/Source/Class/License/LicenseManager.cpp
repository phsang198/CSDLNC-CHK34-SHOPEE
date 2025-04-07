#pragma once 
#include "pch.h"
#include "LicenseManager.h"

// license-master.
#include <iostream>
#include <map>
#include "public_key.h"

#define REPEATED			5



/*===================================================================================================================
========================================== CLicenseManager ========================================================
===================================================================================================================*/

CLicenseManager::CLicenseManager() :
	_Thread("LicenseManager")
{
	_bStopped = true;
	_vKey = PUBLIC_KEY;
}

CLicenseManager::~CLicenseManager()
{
	Stop();
}

void CLicenseManager::run()
{	
	while (!_bStopped)
	{
		try
		{
			if (Check(_szProductName, _szLicenseFile) != LICENSE_OK)
			{
				//CVBDServer::ReportService(_pParent->GetServiceName().c_str(), "license expired or not found!");
				exit(3);
			}
		}
		catch (...)
		{
		}

		Poco::Thread::sleep(1000);
	}
}

void CLicenseManager::Start()
{
	_bStopped = false;
	_Thread.start(*this);
}

void CLicenseManager::Stop()
{
	if (!_bStopped)
	{
		_bStopped = true;
		_Thread.join();
	}
}

void CLicenseManager::SetProductName(const std::string& szProductName)
{
	_szProductName = szProductName;
}

void CLicenseManager::SetLicenseFile(const std::string& szLicenseFile)
{
	_szLicenseFile = szLicenseFile;
}

const std::string& CLicenseManager::GetProductName()
{
	return _szProductName;
}

const std::string& CLicenseManager::GetLicenseFile()
{
	return _szLicenseFile;
}

std::string CLicenseManager::GetHardwareID(const int& nStrategy)
{
	std::string szResult;

	// Get Hardware ID.
	char* pID = nullptr;
	size_t nSize = 0;
	ExecutionEnvironmentInfo envInfo;
	identify_pc((LCC_API_HW_IDENTIFICATION_STRATEGY)nStrategy, pID, &nSize, &envInfo);
	pID = (nSize > 0 ? new char[nSize] : nullptr);
	if (identify_pc((LCC_API_HW_IDENTIFICATION_STRATEGY)nStrategy, pID, &nSize, &envInfo))
		szResult = std::string(pID);
		//szResult.assign(pID);
	if (pID)
		delete[] pID;

	return szResult;
}

int CLicenseManager::Check(const std::string& szProductName, const std::string& szLicense, const int& nType)
{
	CallerInformations callInfo;
	std::copy(szProductName.begin(), szProductName.end(), callInfo.feature_name);
	callInfo.magic = 0;
	callInfo.feature_name[szProductName.size()] = 0;

	LicenseInfo licInfo;
	LicenseLocation licLocation = { (LCC_LICENSE_DATA_TYPE)nType };
	std::copy(szLicense.begin(), szLicense.end(), licLocation.licenseData);

	return acquire_licenseEx(&callInfo, &licLocation, &licInfo, _vKey.data(), static_cast<int>(_vKey.size()));
}

int CLicenseManager::Create(const std::string& szLicenseContents, const std::string& szLicenseFile)
{
	/*CSimpleIniA ini;
	ini.LoadData(szLicenseContents);
	return ini.SaveFile(szLicenseFile.c_str());*/
	Poco::FileOutputStream fos(szLicenseFile, std::ios::binary);
	fos << szLicenseContents;
	fos.close();
	
	return 0;
}

void CLicenseManager::CheckAndCreate(const std::string& szProductName, const std::string& szHardwareID, const std::string& szLicenseFile, int& nRepeated)
{
	if (nRepeated > REPEATED)
		exit(3);

	// Input license key.
	printf("Hardware ID: %s\n", szHardwareID);
	printf("License Key: ");
	std::string szLine;
	std::vector<std::string> vLicense;
	while (std::getline(std::cin, szLine))
	{
		if (std::cin.fail() || szLine.size() == 0)
			break;

		vLicense.push_back(szLine);
	}

	std::string szLicenseContents = vLicense.size() > 0 ? vLicense[0] : "";
	for (int i = 1; i < vLicense.size(); i++)
	{
		szLicenseContents += ("\n" + vLicense[i]);
	}

	// Check license key.
	if (Check(szProductName, szLicenseContents, LICENSE_PLAIN_DATA) != LICENSE_OK)
	{
		std::system("cls");
		nRepeated++;
		CheckAndCreate(szProductName, szHardwareID, szLicenseFile, nRepeated);
	}
	else
		Create(szLicenseContents, szLicenseFile);
}
