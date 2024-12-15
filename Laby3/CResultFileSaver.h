#ifndef CRESULTFILESAVER_H
#define CRESULTFILESAVER_H

#include "ISaver.h"
#include "CResult.h"
#include "CTree.h"
#include "CError.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#define FILESAVER_ERROR_INVALID_TYPE "Invalid type passed to FileSaver"
#define FILESAVER_ERROR_OPEN_FILE "Failed to open file: "
#define FILESAVER_ERROR_PREFIX "Error: "
#define FILESAVER_TREE_PREFIX "Tree Prefix: "
#define DEFAULT_FILE_PATH "output.txt"

template <typename T>
class CResultFileSaver : public ISaver {
public:
    CResultFileSaver(const std::string& filePath);
    virtual bool save(const void* result) const override;
    virtual const std::type_info& getType() const override;

private:
    std::string filePath;
    void saveResult(const CResult<T, CError>& result, std::ofstream& file) const;
    void saveErrors(const std::vector<CError*>& errors, std::ofstream& file) const;
};


template <typename T>
CResultFileSaver<T>::CResultFileSaver(const std::string& filePath) : filePath(filePath) {}

template <typename T>
bool CResultFileSaver<T>::save(const void* result) const {
    std::ofstream file(this->filePath.c_str(), std::ios::app);

    if (!file.is_open()) {
        std::ofstream log(DEFAULT_FILE_PATH, std::ios::app);
        if (log.is_open()) {
            log << FILESAVER_ERROR_OPEN_FILE << this->filePath << std::endl;
            log.close();
        }
        return false;
    }

    const CResult<T, CError>* typedResult = static_cast<const CResult<T, CError>*>(result);
    if (!typedResult) {
        file << FILESAVER_ERROR_INVALID_TYPE << std::endl;
        return false;
    }

    saveResult(*typedResult, file);
    file.close();
}


template <typename T>
const std::type_info& CResultFileSaver<T>::getType() const {
    return typeid(T);
}

template <typename T>
void CResultFileSaver<T>::saveResult(const CResult<T, CError>& result, std::ofstream& file) const {
    if (result.bIsError())
        saveErrors(result.vGetErrors(), file);
    if (result.bIsSuccess()) {
        if (typeid(T) == typeid(CTree*)) {
            const CTree* treeValue = static_cast<const CTree*>(result.cGetValue());
            if (treeValue) {
                file << FILESAVER_TREE_PREFIX << treeValue->getPrefix().cGetValue() << std::endl;
            }
            else {
                file << "Error: Invalid tree value" << std::endl;
            }
        }

    }
}

template <typename T>
void CResultFileSaver<T>::saveErrors(const std::vector<CError*>& errors, std::ofstream& file) const {
    for (size_t i = 0; i < errors.size(); ++i) {
        if (errors[i] && errors[i]->isSavedGet() == false) {
            file << FILESAVER_ERROR_PREFIX << errors[i]->toString() << std::endl;
            errors[i]->isSavedSet(true);
        }
    }
}

#endif 
