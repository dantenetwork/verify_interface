#pragma once

#include <atomic>
#include <thread>
#include <list>

class YThread_11
{
//	friend class std::thread<...>;
	std::atomic<bool> _rFlag;
	std::atomic<bool> _sFlag;
	using YThread_11_void_Func_null = void (YThread_11::*)();
	std::shared_ptr<std::thread> _myThread;
public:
	YThread_11();
	virtual ~YThread_11();

	YThread_11(const YThread_11&) = delete;
	YThread_11& operator = (const YThread_11&) = delete;

	YThread_11(const YThread_11&&) = delete;
	YThread_11& operator = (const YThread_11&&) = delete;

	//Start和Stop最好序列化按顺序调用（即在同一线程中，或在不同线程中上锁调用）
	bool Start();
	bool Stop();

	//用于在自定义的RunningThread中主动退出
	void SelfStop();

	void join();

	virtual void TaskLooping() = 0;

private:
	void _Run();
};

