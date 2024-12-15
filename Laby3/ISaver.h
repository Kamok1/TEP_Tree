#ifndef ISAVER_H
#define ISAVER_H

#include <string>

class ISaver {
public:
    virtual ~ISaver() {}

    virtual bool save(const void* result) const = 0;
    virtual const type_info& getType() const = 0;
};

#endif
