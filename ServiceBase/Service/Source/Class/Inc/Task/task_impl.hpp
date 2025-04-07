//Created: 12 - 06 - 2023

//Modified: 20 - 05 - 2024
//Noted: Update bs_thread_pool 4.1.0

//*reference: https://github.com/bshoshany/thread-pool

#ifndef _TASK_IMPL_20_05_2024_
#define _TASK_IMPL_20_05_2024_

#include <Inc/Noncopyable/noncopyable.hpp>

#define BS_THREAD_POOL_ENABLE_PRIORITY

#ifndef PUSH_TASK
#define PUSH_TASK VBD::Task::Push
#endif // !PUSH_TASK

#ifndef IsTaskDestroy
#define IsTaskDestroy VBD::Task::IsGroupDisable
#endif // !IsTaskDestroy

#ifndef PRIORITY_HIGH
#define PRIORITY_HIGH VBD::Task::pr::high
#endif // !PRIORITY_HIGH

#ifndef PRIORITY_NORMAL
#define PRIORITY_NORMAL VBD::Task::pr::normal
#endif // !PRIORITY_NORMAL

#ifndef PRIORITY_LOW
#define PRIORITY_LOW VBD::Task::pr::low
#endif // !PRIORITY_LOW

#ifndef PRIORITY_LOWEST
#define PRIORITY_LOWEST VBD::Task::pr::lowest
#endif // !PRIORITY_LOWEST

#ifndef PRIORITY_HIGHEST
#define PRIORITY_HIGHEST VBD::Task::pr::highest
#endif // !PRIORITY_HIGHEST

using _Func = std::function<void()>;

namespace VBD::Task
{
	using priority_t = std::int_least16_t;

	namespace pr
	{
		constexpr priority_t highest = 32767;
		constexpr priority_t high = 16383;
		constexpr priority_t normal = 0;
		constexpr priority_t low = -16384;
		constexpr priority_t lowest = -32768;
	}

	//Khởi tạo task pool với số luồng mặc định bằng số luồng cpu * 10
	void Initialize();

	void Push(_Func&& func, std::int_least16_t priority = 0);
	void Clear();
	void OnDestroy(void);
	

	void EnableGroup(uint32_t& nIndex);
	void DisableGroup(const uint32_t& nIndex);
	bool IsGroupDisable(const uint32_t& nIndex);

	void Wait();
	void Status(int& nTaskRunning, int& nTotalTask);

}

#endif // !_TASK_IMPL_