#ifndef FILESAVER_H
#define FILESAVER_H

#include "ISaver.h"
#include <fstream>
#include <typeinfo>
#include <string>
#include "CResult.h"
#include "CTree.h"

#define FILESAVER_ERROR_OPEN_FILE "Error opening file"

class FileSaver : public ISaver {
public:
    FileSaver(const std::string& filePath);
    virtual void save(void* result, const std::type_info& type) const;

private:
    std::string filePath;

    void saveTreeResult(CResult<CTree*, CError>* result, std::ofstream& file) const;
    void saveGenericResult(CResult<void*, CError>* result, std::ofstream& file) const;
    void saveErrors(const std::vector<CError*>& errors, std::ofstream& file) const;
};

#endif
