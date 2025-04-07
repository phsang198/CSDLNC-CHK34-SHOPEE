#include <chrono>
#include <string>
#include <memory>
class ToEpochTime
{
public:
	struct STrigger
	{
		int Seconds_between_Triggers;
	};
	struct MTrigger
	{
		int Minutes_between_Triggers;
	};
	struct HTrigger
	{
		int Hours_between_Triggers;
		int Trigger_at_Minute;     //0 << TaM << 59
	};
	struct DTrigger
	{
		int Days_between_Triggers;
		int Trigger_at_Hour;       //0 << TaH << 23
		int Trigger_at_Minute;     //0 << TaM << 59
	};
public:
	std::string startTime = "";
public: 
	std::string Trigger_Interval;
	STrigger sTrigger;
	MTrigger mTrigger;
	HTrigger hTrigger;
	DTrigger dTrigger;
public:
	time_t get(bool isStart = 1);
private:
	time_t getNextNMinuteHHourEpoch(int N, int H);

	time_t getNextMinuteEpoch(int N);


	time_t getEpoch();

	time_t getEpochAtNMinutesHHoursAfterDDays(int N, int H, int D);

	time_t getEpochAtNAfterHHours(int N, int H);

	time_t getEpochAfterNMinutes(int N);
	time_t getEpochAfterNSeconds(int N);
public:
	static time_t convertToEpoch(const std::string& datetime);
};