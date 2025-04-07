#pragma once

class CProvider; 

class ProviderModel 
{
public: 
	static BOOL getListFileName(CProvider* m_Provider, std::string key, const std::string& substr, std::vector<std::string>& listObjectKey);
	static BOOL getListFile(CProvider* m_Provider, std::string key, const std::string& source, const std::string& des, const std::string& filename, const std::string& substr, const std::string& substr2);
	static BOOL getFile(CProvider* m_Provider, std::string key, std::string fileName);
	//-------------------------------------------------------------------------------------------------------------------------
	static BOOL initData(CProvider* m_Provider, const std::string& dataFloor, const std::string& filename, std::string folderPath);
	static BOOL deleteObject(CProvider* m_Provider, std::string folderPath, std::string filename, std::string subKey);
	static BOOL getData(std::string& data, std::string key, std::string fileName, bool delFolder = 1);
public:
	static CProvider* m_Provider;
	 
};
