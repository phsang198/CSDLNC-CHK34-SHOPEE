#include "pch.h"

#include "task_impl.hpp"
#include "threadpool.hpp"

//TODO: Update template later

namespace VBD::Task
{
	const unsigned int nPool = std::thread::hardware_concurrency() * 2; //Tìm hiểu kĩ hơn về số lượng poolthread với core CPU!  (24-01-2024)

	std::shared_ptr<VBD::thread_pool>			_pThreadPool = nullptr;
	std::unordered_map<uint32_t, bool>		_mDeleted;

	uint32_t	_nIndexGroup = 0;
	bool		_bExit = false;

	bool IsGroupDisable(const uint32_t& nIndex) {
		if (_bExit)
			return true;

		return _mDeleted[nIndex];
	}

	void EnableGroup(uint32_t& nIndex) {
		nIndex = ++_nIndexGroup;
		_mDeleted[nIndex] = false;
	}

	void DisableGroup(const uint32_t& nIndex) {
		_mDeleted[nIndex] = true;
	}

	void Clear() {
		if (_pThreadPool)
		{
			_pThreadPool->purge();
		}
	}

	void OnDestroy(void) {
		Clear();

		_bExit = true;
		_pThreadPool = nullptr;
	}

	void Initialize() {
		if (_pThreadPool == nullptr)
		{
			_pThreadPool = std::make_shared<VBD::thread_pool>(nPool);
		}
		else
		{
			_pThreadPool->purge();
			_pThreadPool->reset();
		}
	}

	void Wait(/*int& nTaskRunning, int& nTotalTask*/)
	{
		if (_pThreadPool)
			_pThreadPool->wait();
	}

	void Status(int& nTaskRunning, int& nTotalTask)
	{
		nTaskRunning = _pThreadPool->get_tasks_running();
		nTotalTask = _pThreadPool->get_tasks_total();

		std::cout << "Task: Running " << nTaskRunning << " | " << nTotalTask << "\n";
	}

	void Push(_Func&& func, std::int_least16_t priority) {
		if (_pThreadPool)
			std::ignore = _pThreadPool->submit_task(std::forward<_Func>(func), priority);
	}

}

