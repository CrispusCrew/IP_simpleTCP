#pragma once

#include <string> 
#include <fstream> 
#include <mutex>       
#include <stdexcept>   
#include <chrono>       
#include <ctime>            

const std::string standartLogPathToFile = "log/log.txt";

class Logger {
private:
    std::mutex logMutex;
    std::ofstream log;
    std::string getSystemTime();
    bool isFileExists(const std::string& pathToFile);
public:
    Logger(const std::string pathToFile);
    ~Logger();
    void logString(const std::string& message);
};