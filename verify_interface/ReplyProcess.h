#pragma once

#include "../pkg/YThread_11.hpp"
#include "../pkg/TSDataQueue.hpp"

#include <cpprest/http_msg.h>

#include <string>
#include <memory>

namespace PlatON_OffChain {
	class ReplyProcess :public YThread_11
	{
		ReplyProcess();
		ReplyProcess(const ReplyProcess&);
		ReplyProcess(ReplyProcess&&);

		ReplyProcess& operator = (const ReplyProcess&);
		ReplyProcess& operator = (ReplyProcess&& );
	public:
		~ReplyProcess();

		static ReplyProcess& GetInftance();

		virtual void TaskLooping();

		void addMessage(const web::json::value& msg);

	private:
		TSDataQueue<web::json::value>	_msgQueue;
	};
}
