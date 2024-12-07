#include "CError.h"
#include "ErrorLevel.h"
#include "ErrorCode.h"

CError::CError(ErrorCode code, ErrorLevel level, const std::string& description)
    : code(code), level(level), description(description) {}


CError::CError(const CError& other)
    : code(other.code), level(other.level), description(other.description) {}

ErrorCode CError::getCode() const {
    return code;
}

ErrorLevel CError::getLevel() const {
    return level;
}

std::string CError::getDescription() const {
    return description;
}

std::string CError::errorLevelToString() const {
    switch (level) {
    case INFO: return "INFO";
    case WARNING: return "WARNING";
    case ERROR: return "ERROR";
    case CRITICAL: return "CRITICAL";
    default: return "UNKNOWN";
    }
}

std::string CError::toString() const {
    std::string result = "Error Code: " + std::to_string(getCode()) + "\n";
    result += "Error Level: " + errorLevelToString() + "\n";
    result += "Error Description: " + getDescription() + "\n";
    result += "--------------------------\n";
    return result;
}