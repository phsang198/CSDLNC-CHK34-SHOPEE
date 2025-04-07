#pragma once

#include "schedule/schedule.h"

class ScheduleModel
{
public:
	ScheduleModel();
	~ScheduleModel();
public:
	int addSchedule(std::shared_ptr<schedule::registration>& reg);
	int addExcuteSchedule(std::string name, std::string config, std::string& id);

	static int updateSchedule(std::shared_ptr<schedule::registration> reg, int type = 0);

	int updateExcuteSchedule(std::string name, std::string config, std::string id);

	int getSchedule();
	int getExcuteSchedule();

	int deleteSchedule(const std::string& id = "");
	int deleteExcuteSchedule(const std::string& id);
};