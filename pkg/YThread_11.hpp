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

	//Start��Stop������л���˳����ã�����ͬһ�߳��У����ڲ�ͬ�߳����������ã�
	bool Start();
	bool Stop();

	//�������Զ����RunningThread�������˳�
	void SelfStop();

	void join();

	virtual void TaskLooping() = 0;

private:
	void _Run();
};

