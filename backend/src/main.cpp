//Include filelogger lib.
#include "../include/FileLogger.h"
#include "../include/Rest.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <chrono>
#include "../include/DbWrapper.h"

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

int ShowMenu()
{
    //Selected option for givin test. and set 1 as default
    int select = 1;

    std::cout << "|-------------------------------------------------|" << std::endl;
    std::cout << "|-     CPPRestFull exemple api - Version 0.0.1   -|" << std::endl;
    std::cout << "|-------------------------------------------------|" << std::endl;
    std::cout << "\nMenu Option:\n"<< std::endl;
    std::cout << "|-------------------------------------------------|" << std::endl;
    std::cout << "|- Option 1 ->  Test a full CRUD operation        |" << std::endl;
    std::cout << "|- Option 2 ->  Test a rest with basic endpoint   |" << std::endl;
    std::cout << "|-------------------------------------------------|" << std::endl;
    std::cout << "\nSelect option[1-2]: ";
    std::cin >> select;
    // Return giving operation.
    return select;
}

void CrudExemple()
{
    // This is the exemple to init psql
    startBenchmark();
    char** result = executeNonPaginateQuery("INSERT INTO program_version (desc_version,version_date) VALUES (2, now())", "I");
    std::cout << "Result from Insert Test: " << result << std::endl;
    finishBenchmark();

}

void CasaBlancaExemple(int argc, char** argv)
{

    // Append default address if has no args seted.
    utility::string_t address = "http://localhost/";
    utility::string_t port = "8000";

    if(argc == 3)
    {
        address = argv[1];
        port = argv[2];
    }

    address.append(port);

    //Init and start HTTP listener.
    Rest *rest = new Rest(address);

    rest->on_initialize(address);

    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    //Cleanup
    rest->on_shutdown();

    delete rest;
    rest = NULL;

}

void SelectMenu(int code, int argc, char** argv)
{
    switch(code)
    {
        case 1:
            CrudExemple();
            break;
        case 2:
            //CasaBlancaExemple();
            break;
        default:
            exit(0);
    }
}

/** Main entry point */
int main(int argc, char* argv[])
{
    int response = ShowMenu();
    SelectMenu(response, argc, argv);
    return 0;
}
