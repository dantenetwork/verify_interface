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

//�Զ����첽�����࣬��std::async��std::launch::asyncģʽ��ʵ�ֵ�Ч��һ����ֻ���Լ�����һ���̳߳أ�
//�����ܹ���Ч���Ʊ������е��߳�������
template <typename ReturnType>
class __YAsync: public HolyAsync
{
	//�������ͼ�飬�����class������ʵ���Ĵ�������������졢������ֵ���ƶ����졢�ƶ���ֵ��
	typedef typename TypeJudge<ReturnType>::type rt;

	//future��������ִ��
	std::future<rt> _m_temp_f;

	//promise���ڴ��ݽ�����������߳�
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
		//async�𵽰󶨺����Ͳ��������ã�������ִ���ӻ���operator()�У���˲���std::launch::deferred����
		_m_temp_f = std::async(std::launch::deferred, cb, args...);
	}

	virtual ~__YAsync(){}

	std::future<rt> async_call()
	{
		std::future<rt> temp_f = _m_promise_f.get_future();

		//����������첽�����̳߳�

		return temp_f;
	}

	//���߳ǳ�������ִ���첽����
	virtual void operator ()()
	{
		rt tmpR = _m_temp_f.get();
		_m_promise_f.set_value(tmpR);
	}
};

//void����������Ҫ�ػ�
template<>
class __YAsync<void> : public HolyAsync
{
	//future��������ִ��
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
		//async�𵽰󶨺����Ͳ��������ã�������ִ���ӻ���operator()�У���˲���std::launch::deferred����
		_m_temp_f = std::async(std::launch::deferred, cb, args...);
	}

	virtual ~__YAsync() {}

	void async_call()
	{
		//��ʵ����ɶ������
	}

	//���߳ǳ�������ִ���첽����
	virtual void operator ()()
	{
		_m_temp_f.get();
	}
};

