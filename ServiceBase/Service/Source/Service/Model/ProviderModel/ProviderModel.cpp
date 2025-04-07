#pragma once 
#include "pch.h"
#include "Provider/Provider.h"
#include "ProviderModel.h"

CProvider* ProviderModel::m_Provider;

//-------------------------------------------------------------------------------------------------------------------------
BOOL ProviderModel::getListFileName(CProvider* m_Provider, std::string key, const std::string& substr, std::vector<std::string>& listObjectKey)
{
	if (m_Provider == NULL) return FALSE; 
	m_Provider->getListNameObject(key, substr, listObjectKey);

	return listObjectKey.empty() ? FALSE : TRUE;
}
BOOL ProviderModel::getListFile(CProvider* m_Provider, std::string key, const std::string& source, const std::string& des, const std::string& filename, const std::string& substr, const std::string& substr2)
{
	if (m_Provider == NULL) return FALSE;

	std::string filezip = filename;

	if (!SupportFunc::existFile(source, filezip))						// kiểm tra đã có chưa , tạo thư mục chứa zip
	{
		std::string buffer = "";

		m_Provider->getListObject(key, source, substr);

		if (substr2 != "")
			m_Provider->getListObject(key, source, substr2);

		SupportFunc::encode(source, des, filezip);
	}
	return TRUE;
}
BOOL ProviderModel::getFile(CProvider* m_Provider, std::string key, std::string fileName)
{
	if (m_Provider == NULL) return FALSE;

	std::string download_path = SupportFunc::getExePath(RUN_PROJECT);

	std::string filePath = download_path + FILE_DATA_TMP + fileName;

	if (!SupportFunc::existFile(download_path + FOLDER_DATA_TMP, fileName))
	{
		std::string buffer = m_Provider->getObject(key + "/" + fileName);
		if (buffer != "")
		{
			std::string fileType = StringProcess::getExtensionFile(fileName);
			ofstream fo;
			if (fileType == "geojson" || fileType == "osm") {
				fo.open(filePath, std::ios::out);
			}
			else {
				fo.open(filePath, std::ios::binary);
			}
			fo << buffer;
			fo.close();
		}
		else
		{
			Poco::Logger::root().information("data from provider NULL", __FILE__, __LINE__);
			return FALSE;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------
BOOL ProviderModel::initData(CProvider* m_Provider, const std::string& data, const std::string& filename, std::string folderPath)
{
	if (m_Provider == NULL) return FALSE;

	std::string exe_path = SupportFunc::getExePath(RUN_PROJECT);
	std::string path = folderPath + "/" + filename;
	//---------------------------------------------------------------------------------------
	BOOL test1 = m_Provider->updateObject(path, data);

	return (test1);
}
BOOL ProviderModel::deleteObject(CProvider* m_Provider, std::string folderPath, std::string filename, std::string subKey)
{
	if (m_Provider == NULL) return FALSE;

	BOOL action = TRUE;
	std::string path;

	if (subKey != "")
	{
		action = m_Provider->deleteListObject(folderPath, subKey);
	}
	else
	{
		path = folderPath + "/" + filename;
		action = m_Provider->deleteObject(path);
	}

	return (action);
}
BOOL ProviderModel::getData(std::string& data, std::string key, std::string fileName, bool delFolder)
{
	std::string download_path = SupportFunc::getExePath(RUN_PROJECT);

	getFile(m_Provider, key, fileName);

	data = SupportFunc::copyContentFile(download_path + FILE_DATA_TMP + fileName);
	if (data != "")
	{
		if (delFolder) SupportFunc::deleteFolder(download_path + FOLDER_DATA_TMP);
		return TRUE;
	}

	return FALSE;
}