#include "utilities.h"

//multithread safe cout
SafeCout scout;

//converting
uint_least16_t safeStringToUint16(const char* str) {
    try {
        uint_least16_t value = std::stoi(str);

        if (value < 0 || value > std::numeric_limits<uint_least16_t>::max()) {
            throw std::out_of_range("Out of range uint_least16_t");
        }

        return value;
    }
    catch (const std::invalid_argument&) {throw std::invalid_argument("Non number char * include");}
    catch (const std::out_of_range&) {throw std::out_of_range("Out of range uint_least16_t");}
}

float safeStdStringToFloat(const std::string& str) {
    try {
        return std::stof(str);
    } 
    catch (const std::invalid_argument& e) {throw std::invalid_argument("Non float char * include");}
    catch (const std::out_of_range& e) {throw std::out_of_range("Out of range float");}
}

int64_t safeFloatToInt64(float flt) {
    try {
        int64_t value = std::round(flt);

        if (flt < static_cast<float>(std::numeric_limits<int64_t>::min()) || 
            flt > static_cast<float>(std::numeric_limits<int64_t>::max())) {
            throw std::out_of_range("Out of range int64_t");
        }

        return value;
    }
    catch (const std::invalid_argument&) {throw std::invalid_argument("Non float input");}
    catch (const std::out_of_range&) {throw std::out_of_range("Out of range int64_t");}
}

float safeStrToFloat(const char* str) {
    return safeStdStringToFloat(std::string(str));
}
