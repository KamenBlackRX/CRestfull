#include <memory>
#include <vector>
#include <sstream>

#include "../include/FileLogger.h"

// Global static pointer used to ensure a single instance of the class.
FileLogger* FileLogger::m_pInstance = NULL;

/* Clean instance inside dctor. */
FileLogger::~FileLogger()
{
        if ( m_pInstance != NULL )
            delete m_pInstance;
}


/* Create a point with class instance and return it to file. */
FileLogger* FileLogger::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new FileLogger;
   return m_pInstance;
}


/* Write log to file with giving message */
void FileLogger::writeToFile(const std::string& message)
{
    // mutex to protect file access (shared across threads)
    static std::mutex mutex;

    // lock mutex before accessing file
    std::lock_guard<std::mutex> lock(mutex);

    // try to open file
    std::ofstream file("Cpprest.log");
    if (!file.is_open())
        throw std::runtime_error("unable to open file");

    // instaciate time and print to file.
    auto timestamp = std::time(nullptr);

    // write message to file
    file << std::ctime(&timestamp) << message << std::endl;

    // file will be closed 1st when leaving scope (regardless of exception)
    // mutex will be unlocked 2nd (from lock destructor) when leaving
    // scope (regardless of exception)

}

/**
 *  Implemented for Gigantes Files(2GB +)
 * send to a string.
 * @return string  Return content of file to string.
*/
template <typename Char, typename Traits,
          typename Allocator = std::allocator<Char>>
auto read_stream_into_string( std::basic_istream<Char, Traits>& in, Allocator alloc = {})
{

    std::basic_ostringstream<Char, Traits, Allocator>
        ss(std::basic_string<Char, Traits, Allocator>(
        std::move(alloc)));

    if (!(ss << in.rdbuf()))
        throw std::ios_base::failure{"error"};

    std::cout << ss.srt();

    return ss.str();
}


/** Read log and Send to string */
std::string FileLogger::readLog()
{
    //lock thread using mutex and make operations.
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    std::ifstream file("Cpprest.log");
    if(!file.is_open())
    {
        throw std::runtime_error("Unable to open file");
    }

    //Iterate from file and show his content. For small files
    auto s = [&file]{
        std::ostringstream ss{};
        ss << file.rdbuf();
        std::cout << ss.str();
        return ss.str();
    }();

    return s;
}

