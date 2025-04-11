#pragma once

#include "Resource/Resource.h"

class OGRFeature;

typedef std::variant<resource> RESOURCETYPE;


class ResourceModel
{
public:
	ResourceModel();
	~ResourceModel();
public:
	int addResource(resource& _Resource);

	int getResource(std::string rscname, std::string auid, std::vector<RESOURCETYPE>& lst_Resource);

	int deleteResource(const std::string rscname, const std::string& auid, const std::string& id = "");
	int Query(const std::string& id, const std::string& geos, const std::string& tablename = "geom_data");
};