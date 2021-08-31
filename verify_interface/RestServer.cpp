#include "RestServer.h"
#include "ReplyProcess.h"

#include <iostream>

RestServer::RestServer(utility::string_t url): _restListener(url)
{
	_restListener.support(methods::GET, std::bind(&RestServer::handle_get, this, std::placeholders::_1));
	_restListener.support(methods::PUT, std::bind(&RestServer::handle_put, this, std::placeholders::_1));
	_restListener.support(methods::POST, std::bind(&RestServer::handle_post, this, std::placeholders::_1));
	_restListener.support(methods::DEL, std::bind(&RestServer::handle_delete, this, std::placeholders::_1));

	auto task = _restListener.open();
	task.wait();
}

RestServer::~RestServer()
{
	_restListener.close().wait();
}

void RestServer::handle_get(http_request message)
{
	try
	{
		json::value res;
		res[U("message")] = json::value::string(U("nice try!"));

		message.reply(status_codes::OK, res);
	}
	catch (const std::exception& e)
	{
		message.reply(status_codes::NotFound);
		std::cout << e.what() << std::endl;
	}
}

void RestServer::handle_put(http_request message)
{
	try
	{
		json::value res;
		res[U("message")] = json::value::string(U("nice try!"));

		message.reply(status_codes::OK, res);
	}
	catch (const std::exception& e)
	{
		message.reply(status_codes::NotFound);
		std::cout << e.what() << std::endl;
	}
}

void RestServer::handle_post(http_request message)
{
	try
	{
		auto uri = message.relative_uri();
		//std::cout << utility::conversions::to_utf8string(uri.path()) << std::endl;

		auto contentTask = message.extract_json();
		//std::cout << utility::conversions::to_utf8string(contentTask.get().serialize());
		PlatON_OffChain::ReplyProcess::GetInftance().addMessage(contentTask.get());

		//reply code
		json::value res;
		res[U("message")] = json::value::string(U("nice try!"));

		message.reply(status_codes::OK, res);
	}
	catch (const std::exception& e)
	{
		message.reply(status_codes::BadRequest);
		std::cout << e.what() << std::endl;
	}
}

void RestServer::handle_delete(http_request message)
{
	try
	{
		json::value res;
		res[U("message")] = json::value::string(U("nice try!"));

		message.reply(status_codes::OK, res);
	}
	catch (const std::exception& e)
	{
		message.reply(status_codes::NotFound);
		std::cout << e.what() << std::endl;
	}
}
