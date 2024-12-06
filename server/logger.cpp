#include "logger.h"

bool Logger::isFileExists(const std::string& pathToFile) {
    std::ifstream file(pathToFile);
    return file.is_open();
}

Logger::Logger (const std::string pathToFile) {

    if (isFileExists(pathToFile))
        log.open(pathToFile, std::ios::app);
    else 
        log.open(pathToFile, std::ios::out);

    if (!log.is_open()) {
        if (isFileExists("../" + pathToFile)) 
            log.open(("../" + pathToFile), std::ios::app);
        else 
            log.open(("../" + pathToFile), std::ios::out);
    }

    if (!log.is_open()) 
        throw std::runtime_error("Error opening file: " + pathToFile);
}

Logger::~Logger () {
    log.close();
}

void Logger::logString(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    log << getSystemTime() << " ";
    log << message << std::endl;
}

std::string Logger::getSystemTime() {
    auto nowTime = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(nowTime);

    std::tm TMsample;

    #ifdef _WIN32
        localtime_s(&TMsample, &in_time_t);  // For Windows
    #else
        localtime_r(&in_time_t, &TMsample);  // For POSIX
    #endif

    // Форматируем дату и время в строку [YYYY-MM-DD HH:MM:SS]
    char timeStr[20]; 
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &TMsample);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime.time_since_epoch()) % 1000;

    std::string resultStr = "[" + std::string(timeStr) + ".";

    resultStr += (std::string(milliseconds.count() < 100 ? "0" : "") + 
                  std::string(milliseconds.count() < 10  ? "0" : "") + 
               	std::to_string(milliseconds.count()) + "]");

    return resultStr;
}