#ifndef ISAVER_H
#define ISAVER_H

#include <typeinfo>
#include <vector>
#include "CError.h"

class ISaver {
public:
    virtual ~ISaver() {}
    virtual std::vector<CError*> save(const void* result) const = 0;
    virtual const std::type_info& getType() const = 0;
};

#endif
