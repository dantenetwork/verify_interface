#pragma once

#include <cpprest/http_listener.h> 

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class RestServer
{
	RestServer(){}
public:
	RestServer(utility::string_t url);
	~RestServer();

protected:
	virtual void handle_get(http_request message);
	virtual void handle_put(http_request message);
	virtual void handle_post(http_request message);
	virtual void handle_delete(http_request message);

private:
	http::experimental::listener::http_listener _restListener;
};

