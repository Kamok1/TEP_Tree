#include "FileSaver.h"
#include "CResult.h"
#include "CError.h"
#include "CTree.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <typeinfo>

FileSaver::FileSaver(const std::string& filePath) : filePath(filePath) {}

void FileSaver::save(void* result, const std::type_info& type) const {
    std::ofstream file(filePath.c_str(), std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error(FILESAVER_ERROR_OPEN_FILE + filePath);
    }

    if (type == typeid(CResult<CTree*, CError>)) {
        CResult<CTree*, CError>* cResult = dynamic_cast<CResult<CTree*, CError>*>(static_cast<CResult<CTree*, CError>*>(result));
        if (cResult) {
            saveTreeResult(cResult, file);
        }
    }
    else {
        CResult<void*, CError>* cResult = dynamic_cast<CResult<void*, CError>*>(static_cast<CResult<void*, CError>*>(result));
        if (cResult) {
            saveGenericResult(cResult, file);
        }
    }

    file.close();
}

void FileSaver::saveTreeResult(CResult<CTree*, CError>* result, std::ofstream& file) const {
    if (result->bIsError()) {
        saveErrors(result->vGetErrors(), file);
    }
    if (result->bIsSuccess()) {
        file << result->cGetValue()->getPrefix().cGetValue() << "\n" << std::endl;
    }
}

void FileSaver::saveGenericResult(CResult<void*, CError>* result, std::ofstream& file) const {
    if (result->bIsError()) {
        saveErrors(result->vGetErrors(), file);
    }
}

void FileSaver::saveErrors(const std::vector<CError*>& errors, std::ofstream& file) const {
    for (int i = 0; i < static_cast<int>(errors.size()); ++i) {
        CError* error = errors[i];
        if (!error->isSavedGet()) {
            error->isSavedSet(true);
            file << error->toString() << std::endl;
        }
    }
}
