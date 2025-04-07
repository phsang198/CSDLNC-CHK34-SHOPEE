#include "pch.h"
#include "CApi.h"

std::map<std::string, CApi> apiManager::lst_api;

CApi::CApi()
{

}
CApi::CApi(std::string _root, std::string _version, std::string _resource, std::string _sub_resource,
	       std::string _method, std::string _attribute, std::string _id)
{
	root = _root;
	version = _version;
	resource = _resource;
	sub_resource = _sub_resource;
	method = _method;
	attribute = _attribute;
	id = _id;
}