#include "CError.h"
#include "ErrorLevel.h"
#include "ErrorCode.h"


CError::CError(ErrorCode code, ErrorLevel level, const std::string& description)
    : code(code), level(level), description(description), isSaved(false) {}


CError::CError(const CError& other)
    : code(other.code), level(other.level), description(other.description), isSaved(other.isSaved)  {}

ErrorCode CError::getCode() const {
    return code;
}

ErrorLevel CError::getLevel() const {
    return level;
}

CError* CError::clone() const {
    return new CError(*this);
}

std::string CError::getDescription() const {
    return description;
}

std::string CError::errorLevelToString() const {
    switch (level) {
    case INFO: return INFO_NAME;
    case WARNING: return WARNING_NAME;
    case ERROR: return ERROR_NAME;
    case CRITICAL: return CRITICAL_NAME;
    default: return DEFAULT_NAME;
    }
}

std::string CError::toString() const { //czy to powinny być stale?
    std::string result = "Error Code: " + std::to_string(getCode()) + "\n";
    result += "Error Level: " + errorLevelToString() + "\n";
    result += "Error Description: " + getDescription() + "\n";
    result += "--------------------------\n";
    return result;
}