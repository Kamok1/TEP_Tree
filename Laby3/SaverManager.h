#ifndef SAVERMANAGER_H
#define SAVERMANAGER_H

#include "ISaver.h"
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include "CError.h"

class SaverManager {
public:
    ~SaverManager();
    bool registerSaver(ISaver* saver);
    bool save(const void* result, const std::type_info& type);
    std::vector<CError*> getErrors() const;

private:
    std::vector<ISaver*> savers;
    std::vector<CError*> errors;
};

#endif
