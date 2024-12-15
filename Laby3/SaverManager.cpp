#include "SaverManager.h"

SaverManager::~SaverManager() {
    for (int i = 0; i < savers.size(); ++i) {
        delete savers[i];
    }
}

bool SaverManager::registerSaver(ISaver* saver) {
    for (int i = 0; i < savers.size(); ++i) {
        if (savers[i]->getType() == saver->getType()) {
            return false; 
        }
    }
    savers.push_back(saver);
    return true;
}

bool SaverManager::save(const void* result, const std::type_info& type) {
    ISaver* defaultSaver = nullptr;

    for (int i = 0; i < savers.size(); ++i) {
        if (savers[i]->getType() == type) {
            std::vector<CError*> errorsFromSaver = savers[i]->save(result);
            if (!errorsFromSaver.empty()) {
                for (int j = 0; j < errorsFromSaver.size(); ++j) {
                    errors.push_back(errorsFromSaver[j]);
                }
                return false;
            }
            return true;
        }
        if (savers[i]->getType() == typeid(void*)) {
            defaultSaver = savers[i];
        }
    }

    if (defaultSaver) {
        std::vector<CError*> errorsFromSaver = defaultSaver->save(result);
        if (!errorsFromSaver.empty()) {
            for (int j = 0; j < errorsFromSaver.size(); ++j) {
                errors.push_back(errorsFromSaver[j]);
            }
            return false;
        }
        return true;
    }
}

std::vector<CError*> SaverManager::getErrors() const {
    return errors;
}
