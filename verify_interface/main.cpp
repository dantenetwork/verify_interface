#include "ReplyProcess.h"
#include "../test/rest_client.h"
#include "RestServer.h"

#include <thread>

int main()
{
	//cpprestTest();
	//testJson();

	//RestServer rs(U("http://192.168.1.85:7890/"));
	//PlatON_OffChain::ReplyProcess::GetInftance().Start();
	
	while (true)
	{
		testSync();

		//std::this_thread::sleep_for(2000ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(15000));
	}

	return 0;
}
