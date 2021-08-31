#pragma once

#include "TypeJudge.hpp"

#include <future>

class HolyAsync
{
public:
	HolyAsync(){}
	virtual ~HolyAsync() = 0
	{

	}

	virtual void operator () () = 0;
};

//自定义异步操作类，与std::async在std::launch::async模式下实现的效果一样，只是自己管理一步线程池，
//这样能够有效控制本进程中的线程总数量
template <typename ReturnType>
class __YAsync: public HolyAsync
{
	//返回类型检查，如果是class，必须实现四大操作（拷贝构造、拷贝赋值、移动构造、移动赋值）
	typedef typename TypeJudge<ReturnType>::type rt;

	//future用于真正执行
	std::future<rt> _m_temp_f;

	//promise用于传递结果给调用者线程
	std::promise<rt> _m_promise_f;
public:
	__YAsync() = delete;
	__YAsync(const __YAsync&) = delete;
	__YAsync& operator = (const __YAsync&) = delete;
	__YAsync(__YAsync&& ) = delete;
	__YAsync& operator = (__YAsync&& ) = delete;

	template<typename Callback, typename ...Args>
	explicit __YAsync(Callback&& cb, Args&&... args)
	{
		//async起到绑定函数和参数的作用，真正的执行延缓到operator()中，因此采用std::launch::deferred参数
		_m_temp_f = std::async(std::launch::deferred, cb, args...);
	}

	virtual ~__YAsync(){}

	std::future<rt> async_call()
	{
		std::future<rt> temp_f = _m_promise_f.get_future();

		//将自身插入异步调用线程池

		return temp_f;
	}

	//在线城池中真正执行异步操作
	virtual void operator ()()
	{
		rt tmpR = _m_temp_f.get();
		_m_promise_f.set_value(tmpR);
	}
};

//void返回类型需要特化
template<>
class __YAsync<void> : public HolyAsync
{
	//future用于真正执行
	std::future<void> _m_temp_f;

public:
	__YAsync() = delete;
	__YAsync(const __YAsync&) = delete;
	__YAsync& operator = (const __YAsync&) = delete;
	__YAsync(__YAsync&&) = delete;
	__YAsync& operator = (__YAsync&&) = delete;

	template<typename Callback, typename ...Args>
	explicit __YAsync(Callback&& cb, Args&&... args)
	{
		//async起到绑定函数和参数的作用，真正的执行延缓到operator()中，因此采用std::launch::deferred参数
		_m_temp_f = std::async(std::launch::deferred, cb, args...);
	}

	virtual ~__YAsync() {}

	void async_call()
	{
		//其实就是啥都不做
	}

	//在线城池中真正执行异步操作
	virtual void operator ()()
	{
		_m_temp_f.get();
	}
};

