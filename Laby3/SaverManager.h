#ifndef SAVERMANAGER_H
#define SAVERMANAGER_H

#include "ISaver.h"
#include <vector>
#include <typeinfo>
#include <stdexcept>

class SaverManager {
public:
    ~SaverManager() {
        for (int i = 0; i < savers.size(); ++i) {
            delete savers[i];
        }
    }

    void registerSaver(ISaver* saver) {
        savers.push_back(saver);
    }

    bool save(const void* result, const std::type_info& type) const {
        ISaver* defaultSaver = nullptr;

        for (int i = 0; i < savers.size(); ++i) {
            if (savers[i]->getType() == type) {
                return savers[i]->save(result);
            }
            if (savers[i]->getType() == typeid(void*)) {
                defaultSaver = savers[i];
            }
        }

        if (defaultSaver) {
            return defaultSaver->save(result);
        }
    }

private:
    std::vector<ISaver*> savers;
};

#endif
