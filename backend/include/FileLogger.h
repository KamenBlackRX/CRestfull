#include <mutex>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>



class FileLogger
{
    public:

    static FileLogger* Instance();

    void writeToFile(const std::string& message);
    bool closeFileLog(); // Will be depreciated with RAII implementations.

    private:
    /* Singleton constructors */
    FileLogger() {};
    FileLogger(FileLogger const&); // Prevent copy
    FileLogger& operator=(FileLogger const&); // Prevent assigment.

    /**
     * Open a desired log file and store his response.
     * @param string  Path for access the file. Needs be writable and readable.
     * @return ofstream  Response for open operation.
     */
    std::ofstream openLogFile(std::string Filepath); // Will be depreciated with RAII implementations.

    static FileLogger* m_pInstance; // Instance for singleton pattern.

};

