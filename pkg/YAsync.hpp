#pragma once

#include "__YAsync.hpp"
#include "YThread_11.hpp"
#include "TSDataQueue.hpp"

#include <functional>

///////////////////////////////////////////////////////////////////////////////////////////
//内部定义
class __async_thread_pool;

class __async_call_thread : public YThread_11
{
public:
	__async_call_thread(){}

	virtual void TaskLooping();
};

class __async_thread_pool
{
	__async_thread_pool(unsigned int tn);

	__async_thread_pool(const __async_thread_pool&) = delete;
	__async_thread_pool& operator = (const __async_thread_pool&) = delete;
	__async_thread_pool( __async_thread_pool&& ) = delete;
	__async_thread_pool& operator = ( __async_thread_pool&& ) = delete;


	TSDataQueue<HolyAsync> _taskQueue;
	std::list<std::shared_ptr<__async_call_thread>> _taskThreads;
public:

	static __async_thread_pool& GetInstance();

	~__async_thread_pool();

	std::shared_ptr<HolyAsync> pop(unsigned int msTimeout = 0xFFFFFFFF);
	void push(std::shared_ptr<HolyAsync> ha);
};

///////////////////////////////////////////////////////////////////////////////////////////
//异步接口

template <typename T>
struct rtDeduction
{
	typedef std::future<T> type;
};

template<>
struct rtDeduction<void>
{
	typedef void type;
};

//异步操作接口，可将任意调用异步化，示例详见futureTest.hpp、futureTest.cpp
//要求如果返回值是用户自定义类型，则必须实现拷贝构造、拷贝赋值、转移构造、转移赋值函数，否则编译错误
template<typename Callback, typename... Args>
auto YAsync_Call(Callback&& bc, Args&&... args)
{
	typedef typename std::result_of_t<std::decay_t<Callback>(std::decay_t<Args>...)> ReturnType;

	std::shared_ptr<HolyAsync> spYAsync(new __YAsync<ReturnType>(bc, args...));

	//这里必须下溯做类型转换
	__YAsync<ReturnType>* ptr = static_cast<__YAsync<ReturnType>*>(spYAsync.get());
	if (nullptr == ptr)
	{
		//这里隐含了某些情况下会返回void()，这是可以的
		return rtDeduction<ReturnType>::type();
	}

	__async_thread_pool::GetInstance().push(spYAsync);

	return ptr->async_call();
}

