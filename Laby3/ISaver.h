#ifndef ISAVER_H
#define ISAVER_H

#include <string>
#include <typeinfo>

class ISaver {
public:
    virtual ~ISaver() {}
    virtual void save(void* result, const std::type_info& type) const = 0;
};

#endif
