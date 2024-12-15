#ifndef CRESULTFILESAVER_H
#define CRESULTFILESAVER_H

#include "ISaver.h"
#include "CResultVoid.h"
#include "CTree.h"
#include "CError.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <typeinfo>


#define FILESAVER_ERROR_INVALID_TYPE "Invalid type passed to FileSaver"
#define FILESAVER_ERROR_OPEN_FILE "Failed to open file: "
#define FILESAVER_ERROR_PREFIX "Error: "
#define FILESAVER_TREE_PREFIX "Tree Prefix: "
#define DEFAULT_FILE_PATH "output.txt"
#define INVALID_TREE_VALUE "Invalid tree value"

template <typename T>
class CResultFileSaver : public ISaver {
public:
    CResultFileSaver(const std::string& filePath);
    virtual std::vector<CError*> save(const void* result) const override;
    virtual const std::type_info& getType() const override;

private:
    std::string filePath;
    void saveResult(const CResult<T, CError>& result, std::ofstream& file) const;
    void saveErrors(const std::vector<CError*>& errors, std::ofstream& file) const;
};

template <typename T>
CResultFileSaver<T>::CResultFileSaver(const std::string& filePath) : filePath(filePath) {}

template <typename T>
std::vector<CError*> CResultFileSaver<T>::save(const void* result) const {
    std::ofstream file(this->filePath.c_str(), std::ios::app);
    std::vector<CError*> errors;

    if (!file.is_open()) {
        errors.push_back(new CError(ERROR_FILE_NOT_FOUND, CRITICAL, FILESAVER_ERROR_OPEN_FILE + filePath));
        return errors;
    }

    const CResult<T, CError>* typedResult = static_cast<const CResult<T, CError>*>(result);
    if (!typedResult) {
        errors.push_back(new CError(ERROR_NONE, CRITICAL, FILESAVER_ERROR_INVALID_TYPE));
        return errors;
    }

    saveResult(*typedResult, file);
    file.close();
    return errors;
}

template <typename T>
const std::type_info& CResultFileSaver<T>::getType() const {
    return typeid(T);
}

template <typename T>
void CResultFileSaver<T>::saveResult(const CResult<T, CError>& result, std::ofstream& file) const {
    if (result.bIsError()) {
        saveErrors(result.vGetErrors(), file);
    }

    if (result.bIsSuccess() && typeid(T) == typeid(CTree*)) {
        const CTree* treeValue = static_cast<const CTree*>(result.cGetValue());
        if (treeValue) {
            file << FILESAVER_TREE_PREFIX << treeValue->getPrefix().cGetValue() << std::endl;
        }
        else {
            file << FILESAVER_ERROR_PREFIX << INVALID_TREE_VALUE << std::endl;
        }
    }
}

template <typename T>
void CResultFileSaver<T>::saveErrors(const std::vector<CError*>& errors, std::ofstream& file) const {
    for (int i = 0; i < errors.size(); ++i) {
        if (errors[i] && !errors[i]->isSavedGet()) {
            file << FILESAVER_ERROR_PREFIX << errors[i]->toString() << std::endl;
            errors[i]->isSavedSet(true);
        }
    }
}
#endif
