#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H

// stl.
#include <mutex>
#include <vector>

// Poco.
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

// license-master.
#include "licensecc_properties.h"
#include "licensecc/licensecc.h"


/*===================================================================================================================
============================================ CLicenseManager ========================================================
===================================================================================================================*/

class CLicenseManager : public Poco::Runnable
{
private:
	bool			_bStopped;
	Poco::Thread	_Thread;
	std::mutex		_mLock;

protected:
	std::string		_szProductName;
	std::string		_szLicenseFile;
	std::vector<unsigned char>	_vKey;

public:
	CLicenseManager();
	virtual ~CLicenseManager();

protected:
	void run();

public:
	void Start();
	void Stop();

	void SetProductName(const std::string& szProductName);
	void SetLicenseFile(const std::string& szLicenseFile);

	const std::string& GetProductName();
	const std::string& GetLicenseFile();
	std::string GetHardwareID(const int& nStrategy = STRATEGY_DEFAULT);

	int Check(const std::string& szProductName, const std::string& szLicense, const int& nType = LICENSE_PATH);
	int Create(const std::string& szLicenseContents, const std::string& szLicenseFile);
	void CheckAndCreate(const std::string& szProductName, const std::string& szHardwareID, const std::string& szLicenseFile, int& nRepeated);
};

#endif // LICENSEMANAGER_H