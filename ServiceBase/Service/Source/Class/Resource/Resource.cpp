#include "pch.h"
#include "resource.h"

//std::map<std::string, resource> resourceManager::lst_resource;
//std::map<std::string, std::string> resourceManager::lst_resource_name;

resource::resource()
{

}
resource::resource(std::string _auid, std::string _rscname, std::string _rscid,
	std::string _belongname, std::string _belongid, std::string _id )
{
	auid = _auid;
	rscname = _rscname;
	rscid = _rscid;
	belongname = _belongname;
	belongid = _belongid;
	id = _id;
}
//
//void resourceManager::id2name()
//{
//	for (auto x : lst_resource)
//	{
//		lst_resource_name[x.second.name] = x.first;
//	}
//}