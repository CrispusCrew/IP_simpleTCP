#pragma once
#include <string>
#include <iostream>
#include <mutex>
#include <cstdint>
#include <math.h>

// converting
float safeStdStringToFloat(const std::string& str);
float safeStrToFloat(const char* str);
int64_t safeFloatToInt64(float flt);
uint_least16_t safeStringToUint16(const char* str);


// multi-thread cout
class SafeCout {
public:
    template<typename T>
    SafeCout& operator<< (const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << value;
        return *this;
    }
    
    //template<typename T>
    SafeCout& operator<<(std::ostream& (*manip)(std::ostream&)) {
        std::lock_guard<std::mutex> lock(mutex_);
        manip(std::cout);
        return *this;
    }

private:
    std::mutex mutex_; 
};

extern SafeCout scout;
