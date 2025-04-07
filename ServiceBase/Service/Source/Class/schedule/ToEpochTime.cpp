//#include "pch.h"
#include "ToEpochTime.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
time_t ToEpochTime::get(bool isStart ) {
	time_t tmp = 1;
	if (Trigger_Interval == "Minutes")
	{
		if (isStart)
			return getEpoch();
		return getEpochAfterNMinutes(mTrigger.Minutes_between_Triggers); 
	}else 
	if (Trigger_Interval == "Seconds")
	{
		if (isStart)
			return getEpoch();
		return getEpochAfterNSeconds(sTrigger.Seconds_between_Triggers);
	}else
	if (Trigger_Interval == "Hours")
	{
		if (isStart)
			return getNextMinuteEpoch(hTrigger.Trigger_at_Minute);
		return getEpochAtNAfterHHours(hTrigger.Trigger_at_Minute, hTrigger.Hours_between_Triggers);
	}
	else
	if (Trigger_Interval == "Days")
	{
		if (isStart)
			return getNextNMinuteHHourEpoch(dTrigger.Trigger_at_Minute,dTrigger.Trigger_at_Hour);
		return getEpochAtNMinutesHHoursAfterDDays(dTrigger.Trigger_at_Minute,dTrigger.Trigger_at_Hour,dTrigger.Days_between_Triggers);
	}

	return tmp;
}
time_t ToEpochTime::getNextNMinuteHHourEpoch(int N, int H) {
	time_t now = time(nullptr);
	tm* now_tm = localtime(&now);

	tm target_tm = *now_tm;
	target_tm.tm_sec = 0;  
	target_tm.tm_min = N; 
	target_tm.tm_hour = H; 

	if (now_tm->tm_hour > H || (now_tm->tm_hour == H && now_tm->tm_min >= N)) {
		target_tm.tm_mday += 1;
	}

	time_t next_epoch = mktime(&target_tm);

	return next_epoch;
}
time_t ToEpochTime::getNextMinuteEpoch(int N) {
	time_t now = time(nullptr);
	tm* now_tm = localtime(&now);

	tm next_tm = *now_tm;
	next_tm.tm_sec = 0;  
	next_tm.tm_min = N; 

	if (now_tm->tm_min > N) {
		next_tm.tm_hour += 1;
		if (next_tm.tm_hour == 24) { 
			next_tm.tm_hour = 0;
			next_tm.tm_mday += 1;
		}
	}
	time_t next_epoch = mktime(&next_tm);

	return next_epoch;
}

time_t ToEpochTime::getEpoch() {
	time_t now = time(nullptr);
	tm* now_tm = localtime(&now);

	now_tm->tm_sec = 0;

	time_t current_minute_epoch = mktime(now_tm);

	return current_minute_epoch;
}
time_t ToEpochTime::getEpochAtNMinutesHHoursAfterDDays(int N, int H, int D) {
	time_t now = time(nullptr);
	tm* now_tm = localtime(&now);

	now_tm->tm_mday += D;

	now_tm->tm_hour = H;
	now_tm->tm_min = N;
	now_tm->tm_sec = 0; 

	time_t future_epoch = mktime(now_tm);

	return future_epoch;
}

time_t ToEpochTime::getEpochAtNAfterHHours(int N, int H) {
	time_t now = time(nullptr);
	tm* now_tm = localtime(&now);

	now_tm->tm_hour += H;

	if (now_tm->tm_hour >= 24) {
		now_tm->tm_hour -= 24;
		now_tm->tm_mday += 1;
	}

	now_tm->tm_min = N;
	now_tm->tm_sec = 0;

	time_t future_epoch = mktime(now_tm);

	return future_epoch;
}
time_t ToEpochTime::getEpochAfterNMinutes(int N)
{
	time_t now = time(nullptr);

	time_t future_epoch = now + (N*60);

	return future_epoch;
}
time_t ToEpochTime::getEpochAfterNSeconds(int N)
{
	time_t now = time(nullptr);

	time_t future_epoch = now + N;
	std::cout << "lich theo giay : " << future_epoch << std::endl;

	return future_epoch;
}

time_t ToEpochTime::convertToEpoch(const std::string& datetime)
{
	tm tm_time = {};

	std::istringstream ss(datetime);
	ss >> std::get_time(&tm_time, "%Y-%m-%d %H:%M:%S");

	tm_time.tm_isdst = -1;

	time_t epoch_time = mktime(&tm_time);

	return epoch_time;
}