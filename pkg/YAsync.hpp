#pragma once

#include "__YAsync.hpp"
#include "YThread_11.hpp"
#include "TSDataQueue.hpp"

#include <functional>

///////////////////////////////////////////////////////////////////////////////////////////
//�ڲ�����
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
//�첽�ӿ�

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

//�첽�����ӿڣ��ɽ���������첽����ʾ�����futureTest.hpp��futureTest.cpp
//Ҫ���������ֵ���û��Զ������ͣ������ʵ�ֿ������졢������ֵ��ת�ƹ��졢ת�Ƹ�ֵ����������������
template<typename Callback, typename... Args>
auto YAsync_Call(Callback&& bc, Args&&... args)
{
	typedef typename std::result_of_t<std::decay_t<Callback>(std::decay_t<Args>...)> ReturnType;

	std::shared_ptr<HolyAsync> spYAsync(new __YAsync<ReturnType>(bc, args...));

	//�����������������ת��
	__YAsync<ReturnType>* ptr = static_cast<__YAsync<ReturnType>*>(spYAsync.get());
	if (nullptr == ptr)
	{
		//����������ĳЩ����»᷵��void()�����ǿ��Ե�
		return rtDeduction<ReturnType>::type();
	}

	__async_thread_pool::GetInstance().push(spYAsync);

	return ptr->async_call();
}

