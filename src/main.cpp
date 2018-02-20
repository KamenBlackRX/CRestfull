//Include filelogger lib.
#include "../include/FileLogger.h"
#include "../include/Rest.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <chrono>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams


using Clock = std::chrono::high_resolution_clock;
std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> t1, t2;

/** StartBenchmark Timer. */
void startBenchmark()
{
    t1 = Clock::now();
}

void finishBenchmark()
{
    t2 = Clock::now();
    std::cout << "Delta t2-t1: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms" << std::endl;
    FileLogger::Instance()->writeToFile(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()));
}

void exemple()
{
  auto fileStream = std::make_shared<ostream>();

    startBenchmark();
    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {

        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://www.bing.com/"));

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
        finishBenchmark();
        requestTask.wait();

    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }

}



int main(int argc, char* argv[])
{
    utility::string_t address = "localhost";
    utility::string_t port = "3570";

    if(argc == 3)
    {
        address = argv[1];
        port = argv[2];
    }

    address.append(port);

    Rest::on_initialize(address);

    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    Rest::on_shutdown();

    return 0;
}
