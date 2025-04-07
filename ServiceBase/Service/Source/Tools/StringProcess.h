#pragma once
#include <string>
#include <regex>
#include <crow.h>
#include <Poco/Timespan.h>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeParser.h>

#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <ctype.h>
#include <codecvt>
#include <algorithm>
#include <vector>

#include <document.h>
#include "filereadstream.h"
#include <rapidjson.h>
#include <document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <prettywriter.h>
#include <boost/lexical_cast.hpp>
using namespace rapidjson;

class StringProcess
{
public:
	//static std::string cs2String(CString cstr)
	//{
	//	CT2CA pszConvertedAnsiString(cstr);
	//	return std::string(pszConvertedAnsiString);
	//}
	static std::string double2string(double n)
	{
		std::string str = boost::lexical_cast<std::string>(n);

		/*std::ostringstream strs;
		strs << n;
		std::string str = strs.str();*/
		return str; 
	}
	static std::string ws2string(std::wstring ws)
	{
		std::string s(ws.begin(), ws.end());
		return s;
	}

	static std::string& Object2String(const rapidjson::Value& value, std::string* str)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		value.Accept(writer);
		str->assign(buffer.GetString(), buffer.GetSize());
		return *str;
	}

	static std::string Object2String(const rapidjson::Value& value)
	{
		std::string str;
		Object2String(value, &str);
		#if __cplusplus < 201103L
		return str;
		#else
		return std::move(str);
		#endif//__cplusplus < 201103L
	}
	static bool ReplaceRegex(std::string& str, const std::string& from, const std::string& to) {
		try
		{
			str = std::regex_replace(str, std::regex(from), to);
			return true;
		}
		catch (const std::exception&)
		{
			return false; 
		}
	}
	static bool ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		while (start_pos != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos = str.find(from);
		}
		
		return true;
	}
	static bool Replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		//str.erase(start_pos, from.length()); 
		//str.insert(start_pos, to); 
		std::string _to = to; 
		if (_to == "") _to = "";
		str.replace(start_pos, from.length(), _to);
		return true;
	}
	static int ConstCharToInt(const char* value)
	{
		std::stringstream strValue;
		strValue << value;
		int intValue;
		strValue >> intValue;
		return intValue;
	}
	static double cs2double(const char* value)
	{
		return std::atof(value);
	}
	static BOOL readFile(std::string path, std::string& content)
	{
		std::ifstream f;
		f.open(path, std::ios::in);
		std::string tmp;
		if (f.fail())
		{
			return FALSE;
		}
		while (std::getline(f, tmp)) {
			content += tmp;
		}
		f.close();
		return TRUE; 
	}
	static std::string UTC2LocalTimeZone(std::string szDateTime , int offset)
	{
		int _nTZD = offset;

		if (offset == -1)
		{
			Poco::LocalDateTime ldtNow;
			_nTZD = ldtNow.tzd();// 25200;
		}

		//Poco::Timespan _tsVN2UTC = Poco::Timespan(-_nTZD, 0);//Poco::Timespan(0, -7, 0, 0, 0);
		//Poco::Timespan _tsUTC2VN = Poco::Timespan(_nTZD, 0);//Poco::Timespan(0, 7, 0, 0, 0);

		int nTZD;
		Poco::DateTime dtValue = Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, szDateTime, nTZD);
		dtValue.makeUTC(nTZD);
		szDateTime = Poco::DateTimeFormatter::format(dtValue, Poco::DateTimeFormat::ISO8601_FORMAT, _nTZD);
		return szDateTime;
	}
	static std::vector<std::string> parseURLToArray(std::string str, const char c)
	{
		std::vector<std::string> result;
		//Find string
		int firstIndex = 0;
		for (int i = 0; i < str.size(); ++i)
		{
			if (str[i] == c)
			{
				std::string subStr = str.substr(firstIndex, i - firstIndex);
				result.push_back(subStr);
				firstIndex = i + 1;
			}
			if (i == str.size() - 1)
			{
				std::string subStr = str.substr(firstIndex, i - firstIndex + 1);
				result.push_back(subStr);
			}
		}
		return result;
	}
	static std::vector<int> parseStrToInt(std::string str)
	{
		const char c = ','; 
		std::vector<int> result; 
		std::vector<std::string> strs = parseURLToArray(str, c);
		for (int i = 0; i < strs.size(); ++i)
		{
			result.push_back(std::stoi(strs.at(i))); 
		}
		return result; 
	}
	static std::vector<double> parseStrToDouble(std::string str,char c)
	{
		std::vector<double> result;
		std::vector<std::string> strs = parseURLToArray(str, c);
		for (int i = 0; i < strs.size(); ++i)
		{
			result.push_back(StringProcess::String2Double(strs.at(i),15));
		}
		return result;
	}
	static double String2Double(std::string const& str, std::size_t const p) {
		std::stringstream sstrm;
		sstrm << std::setprecision(p) << std::fixed << str << std::endl;

		double d;
		sstrm >> d;

		return d;
	}
	static BOOL is_number(const std::string& s)
	{
		return !s.empty() && std::find_if(s.begin(),
			s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
	}
	static std::string getExtensionFile(std::string filename)
	{
		int position = static_cast<int>(filename.find_last_of("."));
		return filename.substr(position + 1);
	}
	static std::string getNameFile(std::string path, char k )
	{
		int position = static_cast<int>(path.find_last_of(k));
		return path.substr(position + 1);
	}
	static std::string getPathExceptNameFile(std::string path)
	{
		int position = static_cast<int>(path.find_last_of('/'));

		std::string filename = path.substr(position);
		path.erase(position, filename.length()); 
		return path; 
	}
	static int existSubStr(std::string str, std::string substr)
	{
		int position = static_cast<int>(str.find(substr));
		return position; 
	}
	static std::string to_string_with_precision(double a_value, const int n = 15)
	{
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}
private:
	StringProcess();
};
