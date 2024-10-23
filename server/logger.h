#include "include.h"

const std::string standartLogPathToFile = "log/log.txt";

class Logger {
private:
    std::ofstream log;
    std::string getSystemTime();
    bool isFileExists(const std::string& pathToFile);
public:
    Logger(const std::string pathToFile);
    ~Logger();
    void logString(const std::string& message);
};