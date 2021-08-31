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
		////默认内存模式是全顺序
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

		//默认内存模式是全顺序
		//if (_leaveFlag.load())
		//{
		//	return;
		//}

		_DataQueue.push_back(sPtrData);

		_dqCond.notify_one();
	}

	//等待的毫秒数
	//如果使用std::chrono::milliseconds(int time)，当msTimeout很大时实际为负值，wait_for直接返回
	//这样线程相当于在空跑，对CPU的消耗会特别大,所以必须用unsiged int
	inline std::shared_ptr<T> pop(unsigned int msTimeout = 0xFFFFFFFF)
	{
		std::unique_lock<std::mutex> ul(_dqLocker);

		//wait的实际意思是，如果前置判断为真，则不等待
		//如果前置判断不为真，则等到为真为止
		//这里必须加上前置判断，否则会出现notify调用在前，wait在后，就有可能永远阻塞了
		//如果队列非空，或已经离开，则无需等待直接返回
		//_dqCond.wait_for(ul, std::chrono::duration<unsigned int, std::ratio<1, 1000>>(msTimeout), [this]() {return (!_DataQueue.empty()) || _leaveFlag.load(); });
		_dqCond.wait_for(ul, std::chrono::milliseconds(msTimeout), [this]() {return (!_DataQueue.empty())/* || _leaveFlag.load()*/; });

		//这里再次判断empty是防止伪唤醒
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

//稀有接口，插入时检查重复，某些特定场景中应用
template<typename T>
class TSDataQueueRare : public TSDataQueue<T>
{
public:
	TSDataQueueRare(){}
	~TSDataQueueRare(){}

	inline void PushWithCheckExist(std::shared_ptr<T> sPtrData, bool update = true)
	{
		std::lock_guard<std::mutex> lg(this->_dqLocker);

		//默认内存模式是全顺序
		//if (_leaveFlag.load())
		//{
		//	return;
		//}

		auto itr = this->_DataQueue.begin();
		while (itr != this->_DataQueue.end())
		{
			if ((**itr) == (*sPtrData))
			{
				//如果重复，更新或者不管
				if (update)
				{
					(*itr) = sPtrData;
				}
				return;
			}

			++itr;
		}

		this->_DataQueue.push_back(sPtrData);
		//只有新数据到来，才POST，重复数据已经在第一次插入时已经POST过了
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

