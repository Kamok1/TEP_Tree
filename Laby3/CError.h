#ifndef CERROR_H
#define CERROR_H

#include <string>
#include "ErrorLevel.h" 
#include "ErrorCode.h"

class CError {
public:
    CError(ErrorCode code, ErrorLevel level, const std::string& description);
    CError(const CError& other);

    ErrorCode getCode() const;
    ErrorLevel getLevel() const;
    std::string getDescription() const;
    std::string toString() const;

private:
    ErrorCode code;
    ErrorLevel level;
    std::string description;
    std::string errorLevelToString() const;
};

#endif
