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
    bool isSavedGet() const {
		return isSaved;
	}
    void isSavedSet(bool isSaved) const{
        		this->isSaved = isSaved;
    }
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
    std::string errorLevelToString() const; 
};

#endif
