#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <atomic>

template<typename T>
class TSDataQueue
{
protected:
	//std::atomic<bool> _leaveFlag;
	std::list<std::shared_ptr<T>> _DataQueue;
	std::mutex _dqLocker;
	std::condition_variable _dqCond;
public:
	TSDataQueue(){
		////Ĭ���ڴ�ģʽ��ȫ˳��
		//_leaveFlag.store(false, std::memory_order_seq_cst);
	}

	virtual ~TSDataQueue(){
		//_leaveFlag.store(true);

		_dqCond.notify_all();

		{
			std::lock_guard<std::mutex> lg(_dqLocker);

			_DataQueue.clear();
		}
	}

	inline void push(std::shared_ptr<T> sPtrData)
	{
		std::lock_guard<std::mutex> lg(_dqLocker);

		//Ĭ���ڴ�ģʽ��ȫ˳��
		//if (_leaveFlag.load())
		//{
		//	return;
		//}

		_DataQueue.push_back(sPtrData);

		_dqCond.notify_one();
	}

	//�ȴ��ĺ�����
	//���ʹ��std::chrono::milliseconds(int time)����msTimeout�ܴ�ʱʵ��Ϊ��ֵ��wait_forֱ�ӷ���
	//�����߳��൱���ڿ��ܣ���CPU�����Ļ��ر��,���Ա�����unsiged int
	inline std::shared_ptr<T> pop(unsigned int msTimeout = 0xFFFFFFFF)
	{
		std::unique_lock<std::mutex> ul(_dqLocker);

		//wait��ʵ����˼�ǣ����ǰ���ж�Ϊ�棬�򲻵ȴ�
		//���ǰ���жϲ�Ϊ�棬��ȵ�Ϊ��Ϊֹ
		//����������ǰ���жϣ���������notify������ǰ��wait�ں󣬾��п�����Զ������
		//������зǿգ����Ѿ��뿪��������ȴ�ֱ�ӷ���
		//_dqCond.wait_for(ul, std::chrono::duration<unsigned int, std::ratio<1, 1000>>(msTimeout), [this]() {return (!_DataQueue.empty()) || _leaveFlag.load(); });
		_dqCond.wait_for(ul, std::chrono::milliseconds(msTimeout), [this]() {return (!_DataQueue.empty())/* || _leaveFlag.load()*/; });

		//�����ٴ��ж�empty�Ƿ�ֹα����
		if (_DataQueue.empty()/* || _leaveFlag.load()*/)
		{
			return std::shared_ptr<T>();
		}

		std::shared_ptr<T> res = _DataQueue.front();
		_DataQueue.pop_front();
		return res;
	}

	inline std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lg(_dqLocker);

		if (_DataQueue.empty()/* || _leaveFlag.load()*/)
		{
			return std::shared_ptr<T>();
		}

		std::shared_ptr<T> res = _DataQueue.front();
		_DataQueue.pop_front();
		return res;
	}
};

//ϡ�нӿڣ�����ʱ����ظ���ĳЩ�ض�������Ӧ��
template<typename T>
class TSDataQueueRare : public TSDataQueue<T>
{
public:
	TSDataQueueRare(){}
	~TSDataQueueRare(){}

	inline void PushWithCheckExist(std::shared_ptr<T> sPtrData, bool update = true)
	{
		std::lock_guard<std::mutex> lg(this->_dqLocker);

		//Ĭ���ڴ�ģʽ��ȫ˳��
		//if (_leaveFlag.load())
		//{
		//	return;
		//}

		auto itr = this->_DataQueue.begin();
		while (itr != this->_DataQueue.end())
		{
			if ((**itr) == (*sPtrData))
			{
				//����ظ������»��߲���
				if (update)
				{
					(*itr) = sPtrData;
				}
				return;
			}

			++itr;
		}

		this->_DataQueue.push_back(sPtrData);
		//ֻ�������ݵ�������POST���ظ������Ѿ��ڵ�һ�β���ʱ�Ѿ�POST����
		this->_dqCond.notify_one();
	}

	inline void GetAllData(std::list<std::shared_ptr<T>>& getDataList)
	{
		std::lock_guard<std::mutex> lg(this->_dqLocker);

		while (!this->_DataQueue.empty())
		{
			auto dataSPtr = this->_DataQueue.front();
			getDataList.push_back(dataSPtr);

			this->_DataQueue.pop_front();
		}
	}
};

