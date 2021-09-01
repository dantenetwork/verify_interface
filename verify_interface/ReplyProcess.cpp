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
	// TODO: �ڴ˴����� return ���
	static ReplyProcess rp;

	return rp;
}

void PlatON_OffChain::ReplyProcess::TaskLooping()
{
	auto msg2process = _msgQueue.pop();

	//��Ӵ�����Ϣ��ҵ���߼�
	//msg2process�Ǹ�std::shared_ptr<json::value>�ṹ
	std::cout << utility::conversions::to_utf8string(msg2process->serialize()) << std::endl;
}

void PlatON_OffChain::ReplyProcess::addMessage(const web::json::value& msg)
{
	std::shared_ptr<web::json::value> msg_sPtr(new web::json::value(msg));

	_msgQueue.push(msg_sPtr);
}
