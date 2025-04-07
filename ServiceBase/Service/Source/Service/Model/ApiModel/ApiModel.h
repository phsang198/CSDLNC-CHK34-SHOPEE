#pragma once

#include "CApi/CApi.h"
class ApiModel
{
public:
	ApiModel();
	~ApiModel();
public:
	int addApi(CApi& _api);
	int updateApi(CApi _api);
	int getApi(std::vector<CApi>& lst_api);
	int deleteApi(const std::string& id);
};