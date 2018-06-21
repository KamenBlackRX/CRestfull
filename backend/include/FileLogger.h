#pragma once

#include <mutex>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>



class FileLogger
{
    public:

    /* Static instance for Singleton access */
    static FileLogger* Instance();

    /**
     * Write log to file with default configuration.
     * @param string message A string reference for log content.
    */
    void writeToFile(const std::string& message);

    /**
     * Write log to file with level verbosity.
     * @param string message A string reference for log content.
    */
    void writeToFile(const std::string& messagem, const std::string& _level);

    /**
     * Find the Log name by searching the sufix in current path 
     */
    void findLogName(std::string sufix, std::string* LogName);


    /* Read all log content from file,
     * send to stdoud and store in a string.
     * @return string  Return content of file to string.
    */
    std::string readLog();

    // public variables
    std::string LogName;

    private:

    /* Singleton constructors */
    FileLogger() {};
    FileLogger(FileLogger const&); // Prevent copy
    FileLogger& operator=(FileLogger const&); // Prevent assigment.

    /* Clean instance before exit. */
    ~FileLogger();

    static FileLogger* m_pInstance; // Instance for singleton pattern.

};

