#pragma once
#include <string>
#include <fstream>

#include <experimental/filesystem>
#include <date/date.h>
#include <Poco/Logger.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include "Poco/Delegate.h"
#include <Poco/Zip/Compress.h>

class SupportFunc
{
private:
	SupportFunc() {};
	~SupportFunc() {};

public:
	static std::string getExePath( std::string projectName)
	{
		char curPath[FILENAME_MAX];
		GetModuleFileNameA(NULL, curPath, sizeof(curPath));
		std::string szCurDir(curPath);

		size_t found = szCurDir.find(projectName);
		szCurDir.erase(found, szCurDir.length() - 1);
		return szCurDir;
	}

	static BOOL existFile(std::string folder, std::string filename)
	{
		Poco::File aFile(folder);
		if (!aFile.exists())
		{
			aFile.createDirectories();
		}

		/*BOOL tmp = std::experimental::filesystem::exists(folder);

		if (!tmp) mkdir(folder.c_str()); */

		std::string path = folder + '/' + filename;
		std::ifstream isf(path);
		return isf.good();
	}
	static BOOL deleteFile(std::string path)
	{
		int ret = remove(path.c_str());
		BOOL is_ok = (ret == 0) ? TRUE : FALSE;
		return is_ok;
	}
	static std::string copyContentFile(std::string path)
	{
		std::ifstream input(path, std::ios::binary);
		std::ostringstream ostrm;
		ostrm << input.rdbuf();
		return std::string(ostrm.str());
	}
	static int numberOfFileInDirectory(std::string path)
	{
		int count = 0;
		for (auto& p : std::experimental::filesystem::directory_iterator(path)) {
			count++;
		}
		return count;
	}

	static BOOL deleteFolder(std::string path)
	{
		{
			Poco::File aFile(path);
			if (aFile.exists() && aFile.isDirectory())
			{
				aFile.remove(TRUE);
				/*if (!aFile.exists())
				{
					aFile.createDirectory();
					return TRUE;
				}*/
				return TRUE;
			}
		}
		return FALSE;
	}

	static void encode(std::string source, std::string des, std::string filename)
	{
		int numberFiles = SupportFunc::numberOfFileInDirectory(source);
		if (numberFiles == 0) return;
		std::ofstream out(des + "/" + filename, std::ios::binary);
		Poco::Zip::Compress c(out, TRUE);

		Poco::File aFile(source);


		if (aFile.exists())
		{
			Poco::Path anEntry(source);
			if (aFile.isDirectory())
			{
				anEntry.makeDirectory();
				c.addRecursive(anEntry, Poco::Zip::ZipCommon::CL_MAXIMUM); // add root if we have more than one entry
			}
			else
			{
				anEntry.makeFile();
				c.addFile(anEntry, anEntry);
			}
		}
		c.close();

	}
	template <typename T1>
	static void vector2map(std::vector<T1> _vector, std::map<std::string, T1>& _map)
	{
		for (auto x : _vector)
		{
			_map[x.id] = x; 
		}
	}
};