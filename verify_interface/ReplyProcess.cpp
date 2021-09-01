#include "ReplyProcess.h"

#include <iostream>

PlatON_OffChain::ReplyProcess::ReplyProcess()
{

}

PlatON_OffChain::ReplyProcess::~ReplyProcess()
{
}

PlatON_OffChain::ReplyProcess& PlatON_OffChain::ReplyProcess::GetInftance()
{
	// TODO: 在此处插入 return 语句
	static ReplyProcess rp;

	return rp;
}

void PlatON_OffChain::ReplyProcess::TaskLooping()
{
	auto msg2process = _msgQueue.pop();

	//添加处理消息的业务逻辑
	//msg2process是个std::shared_ptr<json::value>结构
	std::cout << utility::conversions::to_utf8string(msg2process->serialize()) << std::endl;
}

void PlatON_OffChain::ReplyProcess::addMessage(const web::json::value& msg)
{
	std::shared_ptr<web::json::value> msg_sPtr(new web::json::value(msg));

	_msgQueue.push(msg_sPtr);
}
