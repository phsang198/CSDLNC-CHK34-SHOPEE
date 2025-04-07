#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <Inc/Task/task_impl.hpp>
#include <atomic>
#include "ToEpochTime.h"

namespace schedule
{
	class Timer {
	private:
		struct ScheduledTask {
			std::function<void()> task;
			int64_t scheduledTime;

			bool operator>(const ScheduledTask& other) const {
				return scheduledTime > other.scheduledTime;
			}
		};

		std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, std::greater<ScheduledTask>> tasks;
		std::mutex tasksMutex;
		std::condition_variable cv;
		bool stopping = false;

		std::thread TScheduler; 

	public:
		void addTask(std::function<void()> task, int64_t epochSeconds)
		{
			auto scheduledTime = epochSeconds;

			{
				std::lock_guard<std::mutex> lock(tasksMutex);
				tasks.push({ task, scheduledTime });
			}
		}

		void run() {
			auto wrap = [&]() {
				while (!stopping) 
				{
					//std::cout << tasks.size() << "\n";
					if (!tasks.empty())
					{
						auto tmp = std::chrono::system_clock::now();
						int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(tmp.time_since_epoch()).count() / 1000;

						std::lock_guard<std::mutex> lock(tasksMutex);

						if (now >= tasks.top().scheduledTime)
						{
							auto task = tasks.top().task;
							tasks.pop();

							///std::cout << "push task " << std::endl;
							VBD::Task::Push(std::move(task));
						}
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(5));



				}
			};
			TScheduler = std::move(std::thread(wrap));
		}

		void stop() {
			stopping = true;
			cv.notify_one();
		}

	};

	struct wserver
	{
		static std::string host;
		static std::string WFSVPort ;
	};

	struct sinstance
	{
		std::string iid;
		std::string input; 
		int64_t starttime;
		std::string status;

		std::string token; 
		int itimeout;

		sinstance(std::string _iid, std::string _input, int64_t _starttime);

	};

	class registration
	{
	public:
		enum state {
			Starting,
			Running,
			Stopping,
			Deleted,
			Unknown,
			Stopped,
			Expired
		};

	public:
		registration();
		registration(std::string , std::string ,int , int , std::string , std::string = "");
		registration(std::string , std::string, std::string, std::string, ToEpochTime, std::string = "");
		~registration();
	public:
		std::string scdname; 
		std::string scdid;
		std::string wid;
		int itimeout;
		int wtimeout;
		std::vector<std::shared_ptr<sinstance>> li; 
		std::string inss; 
	public: 
		std::string created; 
	public:
		std::string url;
		std::string method;
		std::string authorization; 
		std::string body;
		ToEpochTime timepoint;

		std::atomic<state> state{Starting};
		std::string getState()
		{
			switch (state)
			{
				case Starting:
					return "Starting";
				case Stopping:
					return "Stopping";
				case Running:
					return "Running";
				case Stopped:
					return "Stopped";
				case Expired:
					return "Expired";
			default:
				break;
			}
			return "Unknown";
		}
	public:
		void parse_ins();
		void li2json();

		void runTask(sinstance* ins);
	public:
		static std::shared_ptr<schedule::registration> parserExcuteSchedule(std::string _body);

		void excute();
		void tryFunc();

	};
	
	class manager
	{
	public:
		static std::map<std::string, std::unordered_map<std::string, std::shared_ptr<schedule::registration>>> m_schedule;
		static std::unordered_map<std::string,std::shared_ptr<schedule::registration>> m_excuteSchedule;
	
		static Timer timer; 
	public: 
		static void add(std::shared_ptr<schedule::registration> reg);
		//--------------------------------------------------------------------------------------
		static void initFrequent(std::shared_ptr<schedule::registration> reg);

		static void addTimePoint(std::shared_ptr<schedule::registration> reg);

		static std::shared_ptr<schedule::registration> getFrequent(std::string_view scdid);

		static int stopFrequent(std::string); 
		static int restartFrequent(std::string); 
		static int tryFunc(std::string);
	public:
		static void addFrequent(std::shared_ptr<schedule::registration> reg);

		//--------------------------------------------------------------------------------------
		static void deleteIs(std::string = "", std::string = "");
		static std::shared_ptr<schedule::registration> updateIStatus(std::string, std::string, std::string, std::string = "");
		static int deleteExcute(std::string scdid = "");
		static std::string getstatus(std::string);
	};
};
