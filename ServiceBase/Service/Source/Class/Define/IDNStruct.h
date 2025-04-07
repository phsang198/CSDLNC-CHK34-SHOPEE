#pragma once

#include "ConfigDefine.h"


struct  Time
{
public:
	Time()
	{
		hour = 0;
		minute = 0;
		second = 0;
	}
	Time(int _hour, int _minute, int _second)
	{
		hour = _hour;
		minute = _minute;
		second = _second;
	}
	~Time() {

	}
public:
	int hour;
	int minute;
	int second;
};

