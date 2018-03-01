//Include filelogger lib.
#include "../include/FileLogger.h"
#include "../include/Rest.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <chrono>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ios>
#include "../include/DbWrapper.h"

using namespace utility;              // Common utilities like string conversions
using namespace web;                  // Common features like URIs.
using namespace web::http;            // Common HTTP functionality
using namespace web::http::client;    // HTTP client features
using namespace concurrency::streams; // Asynchronous streams

// Set clock default
using Clock = std::chrono::high_resolution_clock;
std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> t1, t2;

// static Struct with default constructor
template <typename T>
struct MemoryMapping
{

    //convert All types for string
    template<typename K>
    std::string ToString(K k)
    {

        std::stringstream ss;
        ss << k;
        return ss.str();
    }

    /**
     * Return a report with memory resume
     * @param std::string flag  Flag to set type of memory count (Kb, Mb, Gb)
     * @return T  Return a generic type.
     */
    T GetMemoryResume(std::string flag)
    {
        // Recive as String
        try
        {

            std::string memory = ToString<size_t>(GetCurrentRSSMemory());

            if (typeid(T).name() == "Ss")
            {

                if (flag == "M")
                {
                    std::cout << "Memory Virtual: " << memory << " Mb." << std::endl;
                    std::cout << "Memory Residential: " << memory << " Mb." << std::endl;
                }
                else
                {

                    std::cout << "Memory Virtual: " << memory << " Kb." << std::endl;
                    std::cout << "Memory Residential: " << memory << " Kb." << std::endl;
                }

                return memory;
            }
             //
            if (typeid(T).name() == "size_t")
            {
                std::cout << "Not Imeplemented.";
            }
        }
        catch(std::exception& ex)
        {
            std::cout << "Execption has raised.\nDetails: " << ex.what() << std::endl;
        }


    }

    /**
     * Get Curren Residensial memory with current PID
     * @param int* pid  Current given PID to revice memory mapping
     * @return size_t  Return current Rss value from memory mapping kernel.
     */
    size_t GetCurrentRSSMemory()
    {
        FILE *fp = NULL;
        long rss = 0L;

        if ((fp = fopen("/proc/self/statm", "r")) == NULL)
            return (size_t)0L; /* Can't open? */

        if (fscanf(fp, "%*s%ld", &rss) != 1)
        {
            fclose(fp);
            return (size_t)0L; /* Can't read? */
        }

        fclose(fp);
        return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
    }
};


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
    }
    ).then([=](http_response response)
    {
        // Handle response headers arriving.
        printf("Received response status code:%u\n", response.status_code());
        // Write response body into the file.
        return response.body().read_to_end(fileStream->streambuf());
    }
    ).then([=](size_t)
    {
        // Close the file stream.
        return fileStream->close();
        finishBenchmark();
    }
    );
    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
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
    std::cout << "\nMenu Option:\n"
              << std::endl;
    std::cout << "|-------------------------------------------------|" << std::endl;
    std::cout << "|- Option 1 ->  Test a full CRUD operation        |" << std::endl;
    std::cout << "|- Option 2 ->  Test a rest with basic endpoint   |" << std::endl;
    std::cout << "|- Option 3 ->  Run a test as client rest         |" << std::endl;
    std::cout << "|-------------------------------------------------|" << std::endl;
    std::cout << "\nSelect option[1-3]: ";
    std::cin >> select;
    // Return giving operation.
    return select;
}

void CrudExemple()
{
    // This is the exemple to init psql
    startBenchmark();
    char **result = executeNonPaginateQuery("INSERT INTO program_version (desc_version,version_date) VALUES (2, now())", "I");
    std::cout << "Result from Insert Test: " << result << std::endl;
    finishBenchmark();
}

int CasaBlancaExemple(int argc, char **argv)
{

    // Append default address if has no args seted.
    utility::string_t address = "http://localhost/";
    utility::string_t port = "8000";

    if (argc == 3)
    {
        address = argv[1];
        port = argv[2];
    }

    address.append(port);

    //Init and start HTTP listener.
    Rest *rest = new Rest(address);

    rest->on_initialize(address);

    std::string line;
    std::cout << "Press ENTER to exit." << std::endl;
    MemoryMapping<std::string>*  mmapping = new MemoryMapping<std::string>();
    std::getline(std::cin, line);


    while (line == "")
    {

        std::cout << "===================================" << std::endl;
        std::cout << "| " << mmapping->GetMemoryResume("M") << " |" << std::endl;
        std::cout << "===================================" << std::endl;
        system("sleep 1");
    }

    //Cleanup
    rest->on_shutdown();

    delete rest, mmapping;
    rest = NULL;

    return 0;
}

void SelectMenu(int code, int argc, char **argv)
{
    switch (code)
    {
    case 1:
        CrudExemple();
        break;
    case 2:
        CasaBlancaExemple(argc, argv);
        break;
    case 3:
        exemple();
    default:
        exit(0);
    }
}

/** Main entry point */
int main(int argc, char *argv[])
{
    std::cout << "Show logs: " << std::endl;
    FileLogger::Instance()->readLog();
    int response = ShowMenu();
    SelectMenu(response, argc, argv);

    return 0;
}
