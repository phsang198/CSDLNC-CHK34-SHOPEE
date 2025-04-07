#include "pch.h"
#include "mesh.h"

std::string mesh::host;
std::string mesh::thishost;
std::string mesh::config;
std::string mesh::created;

std::string mesh::getISO8601Timestamp() {
	auto now = std::chrono::system_clock::now();
	auto time_t_now = std::chrono::system_clock::to_time_t(now);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::ostringstream oss;
	oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%dT%H:%M:%S");
	oss << '.' << std::setw(3) << std::setfill('0') << milliseconds.count() << 'Z';

	return oss.str();
}

#define paramGet cpr::Url{ sURL }, cpr::Timeout(1500), cpr::ConnectTimeout(1500), cpr::AcceptEncoding{cpr::AcceptEncodingMethods::disabled }
#define paramPost cpr::Url{ sURL },cpr::Body{ config }, cpr::Timeout(1500), cpr::ConnectTimeout(1500), cpr::AcceptEncoding{cpr::AcceptEncodingMethods::disabled }

BOOL mesh::registerMesh()
{
	std::string sURL = host + "/servicemesh/v1/register";
	cpr::Response r = cpr::Post(paramPost);
	try
	{
		if (r.text != "")
		{
			json p = json::parse(r.text);
			if (p.contains("code")) return FALSE; 

			created = p["summary"]["created"]; 
			thishost = p["summary"]["host"];
			return TRUE;
		}
	}
	catch (const std::exception&)
	{

	}
	return FALSE;
}