#pragma once

#include "Resource/Resource.h"
#include "schedule/schedule.h"

class OGRFeature;

typedef std::variant<resource, std::shared_ptr<schedule::registration>> RESOURCETYPE;


class ResourceModel
{
public:
	ResourceModel();
	~ResourceModel();
public:
	int addResource(resource& _Resource);

	RESOURCETYPE createSchedule(OGRFeature* poFeature);

	RESOURCETYPE createExcuteSchedule(OGRFeature* poFeature);

	int getResource(std::string rscname, std::string auid, std::vector<RESOURCETYPE>& lst_Resource);

	int deleteResource(const std::string rscname, const std::string& auid, const std::string& id = "");
	int Query(const std::string& id, const std::string& geos, const std::string& tablename = "geom_data");
};