#ifndef DETAILEDERROR_H
#define DETAILEDERROR_H

#include "CError.h"
#include "ErrorCode.h"
#include "ErrorLevel.h"
#include <string>
#define DEFAULT_DESCRIPTION "Default description"
#define DEFAULT_CONTEXT "Default context"

class CDetailedError : public CError {
public:
    CDetailedError();
    CDetailedError(ErrorCode code, ErrorLevel level, const std::string& description, const std::string& context);
    CDetailedError(const CDetailedError& other);

    std::string getContext() const;
    virtual std::string toString() const override;
    virtual CDetailedError* clone() const override;
    virtual CDetailedError* copy() const override {
        return new CDetailedError(*this);
    }

private:
    std::string context;
};

#endif
