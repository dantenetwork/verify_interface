#include "YAsync.hpp"

#include <iostream>

void __async_call_thread::TaskLooping()
{
	std::shared_ptr<HolyAsync> ha = __async_thread_pool::GetInstance().pop();
	if (ha.get() != nullptr)
	{
		(*ha)();
	}
}

//////////////////////////////////////////////////////////////////////////////////////

__async_thread_pool::__async_thread_pool(unsigned int tn)
{
	for (unsigned int i = 0; i < tn; i++)
	{
		std::shared_ptr<__async_call_thread> act(new __async_call_thread());
		act->Start();
		_taskThreads.push_back(act);
	}

	std::cout << "Y async backgroud working started!" << std::endl;
}

__async_thread_pool& __async_thread_pool::GetInstance()
{
	static  __async_thread_pool atp(4);
	return atp;
}

__async_thread_pool::~__async_thread_pool()
{
	for (auto act : _taskThreads)
	{
		act->Stop();
	}
}

std::shared_ptr<HolyAsync> __async_thread_pool::pop(unsigned int msTimeout)
{ 
	return _taskQueue.pop(msTimeout);
}

void __async_thread_pool::push(std::shared_ptr<HolyAsync> ha) 
{
	_taskQueue.push(ha);
}
