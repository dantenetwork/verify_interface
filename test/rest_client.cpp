#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "rest_client.h"

#include <iostream>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

int cpprestTest()
{
	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
		{
			*fileStream = outFile;

			// Create http_client to send the request.
			http_client client(U("http://www.bing.com.cn/"));

			// Build request URI and start the request.
			uri_builder builder(U("/search"));
			builder.append_query(U("q"), U("cpprestsdk github"));
			return client.request(methods::GET, builder.to_string());
		})
		// Handle response headers arriving.
		.then([=](http_response response)
			{
				printf("Received response status code:%u\n", response.status_code());

				// Write response body into the file.
				return response.body().read_to_end(fileStream->streambuf());
			})
		// Close the file stream.
		.then([=](size_t)
			{
				return fileStream->close();
			});

		// Wait for all the outstanding I/O to complete and handle any exceptions
		try
		{
			requestTask.wait();
		}
		catch (const std::exception& e)
		{
			printf("Error exception:%s\n", e.what());
		}

		return 0;
}

void testJson()
{
	json::value jv;
	jv[U("id")] = json::value::number(7777);
	jv[U("content")] = json::value::string(U("Hello Dante."));

	json::value jv2 = json::value::parse(utility::conversions::to_utf8string(jv.serialize()));

	std::cout << utility::conversions::to_utf8string(jv2.serialize()) << std::endl;
}

int testSync()
{
	http_client client(U("http://127.0.0.1:7890/"));
	uri_builder builder(U("/Dante"));
	
	json::value jv;
	jv[U("id")] = json::value::number(7777);
	jv[U("content")] = json::value::string(U("Hello Dante."));

	auto responseTask = client.request(methods::POST, builder.to_string(), jv);

	try {
		std::cout << responseTask.get().status_code() << std::endl;
		utility::string_t const& v = responseTask.get().extract_string().get();
		std::cout << utility::conversions::to_utf8string(v) << std::endl;
	}
	catch (http_exception const& e) {
		printf("Error exception:%s\n", e.what());
	}

	return 0;
}

