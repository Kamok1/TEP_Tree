#ifndef CERROR_H
#define CERROR_H

#include <string>
#include "ErrorLevel.h" 
#include "ErrorCode.h"
#include "ISaver.h"

class ISaver;
extern ISaver* globalSaver;

class CError {
public:
    CError(ErrorCode code, ErrorLevel level, const std::string& description);
    CError(const CError& other);

    ErrorCode getCode() const;
    ErrorLevel getLevel() const;
    std::string getDescription() const;
    virtual std::string toString() const;
    virtual ~CError() {}
    virtual CError* clone() const;
    virtual CError* copy() const {
        return new CError(*this);
    }
private:
    ErrorCode code;
    ErrorLevel level;
    std::string description;
    mutable bool isSaved = false; 
    void trySave() const;
    std::string errorLevelToString() const; 
};

#endif
