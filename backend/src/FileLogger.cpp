#include "../include/FileLogger.h"

// Global static pointer used to ensure a single instance of the class.
FileLogger* FileLogger::m_pInstance = NULL;

FileLogger* FileLogger::Instance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new FileLogger;
   return m_pInstance;
}


/**
 * Write log to file with default configuration.
 * @param string message A string reference for log content.
 */
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

