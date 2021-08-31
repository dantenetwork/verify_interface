#include "ReplyProcess.h"
#include "../test/rest_client.h"
#include "RestServer.h"

#include <thread>

int main()
{
	//cpprestTest();
	//testJson();

	RestServer rs(U("http://127.0.0.1:7890/"));
	
	while (true)
	{
		testSync();

		//std::this_thread::sleep_for(2000ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	return 0;
}
