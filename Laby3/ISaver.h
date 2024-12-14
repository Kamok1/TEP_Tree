#ifndef ISAVER_H
#define ISAVER_H

#include <string>
#include "CError.h"

class CError;

class ISaver {
public:
    virtual ~ISaver() {};
    virtual void save(const CError& error) const = 0;
};

#endif
