#include "YThread_11.hpp"

YThread_11::YThread_11()
{
	_rFlag.store(false, std::memory_order_seq_cst);
	_sFlag.store(true, std::memory_order_seq_cst);
}

YThread_11::~YThread_11()
{
	while (!Stop())
	{
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	_myThread->join();
}

bool YThread_11::Start()
{
	if (_rFlag.load(std::memory_order_seq_cst))
	{
		return true;
	}

	_myThread = std::shared_ptr<std::thread>(new std::thread(&YThread_11::_Run, this));

	while (!_rFlag.load(std::memory_order_seq_cst))
	{
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return true;
}

bool YThread_11::Stop()
{
	if (_sFlag.load(std::memory_order_seq_cst))
	{
		return true;
	}

	_rFlag.store(false, std::memory_order_seq_cst);

	while (!_sFlag.load(std::memory_order_seq_cst))
	{
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return true;
}

void YThread_11::_Run()
{
	//这里两个的顺序很重要
	_sFlag.store(false, std::memory_order_seq_cst);
	_rFlag.store(true, std::memory_order_seq_cst);

	while (_rFlag.load(std::memory_order_seq_cst))
	{
		this->TaskLooping();
	}

	_sFlag.store(true, std::memory_order_seq_cst);
}

void YThread_11::SelfStop()
{
	_rFlag.store(false, std::memory_order_seq_cst);
}

void YThread_11::join()
{
	_myThread->join();
}
