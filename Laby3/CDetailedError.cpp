#include "CDetailedError.h"

CDetailedError::CDetailedError()
    : CError(ErrorCode::ERROR_NONE, ErrorLevel::INFO, DEFAULT_DESCRIPTION), context(DEFAULT_CONTEXT) {}

CDetailedError::CDetailedError(ErrorCode code, ErrorLevel level, const std::string& description, const std::string& context)
    : CError(code, level, description), context(context) {}

CDetailedError::CDetailedError(const CDetailedError& other)
    : CError(other), context(other.context) {}

std::string CDetailedError::getContext() const {
    return context;
}

CDetailedError* CDetailedError::clone() const {
    return new CDetailedError(*this);
}

std::string CDetailedError::toString() const { //czy to powinny być stale?
    std::string baseString = CError::toString();
    baseString += "Error Context: " + context + "\n";
    baseString += "==========================\n";
    return baseString;
}
