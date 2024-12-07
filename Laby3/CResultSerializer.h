#ifndef CRESULTSERIALIZER_H
#define CRESULTSERIALIZER_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "CResultVoid.h"
#include "CError.h"
#include "CTree.h"


template <typename T>
class CResultSerializer {
public:
    static CResult<void, CError> serialize(const CResult<T, CError>& result, const std::string& filename);

private:
    static void serializeFailures(const std::vector<CError*>& errors, std::ofstream& ofs);
};

template <typename T>
void CResultSerializer<T>::serializeFailures(const std::vector<CError*>& errors, std::ofstream& ofs) {
    ofs << "Failure" << std::endl;
    for (const auto& error : errors) {
        ofs << error->toString() << std::endl;
    }
}

template <typename T>
CResult<void, CError> CResultSerializer<T>::serialize(const CResult<T, CError>& result, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        return new CError(ERROR_FILE_NOT_FOUND, CRITICAL, "Failed to open file: " + filename);
    }

    if (result.bIsSuccess()) {
        ofs << "Success" << std::endl;
        ofs << result.cGetValue().toString() << std::endl;
    }
    else {
        serializeFailures(result.vGetErrors(), ofs);
    }

    ofs.close();
}

template <>
CResult<void, CError> CResultSerializer<CTree*>::serialize(const CResult<CTree*, CError>& result, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        return new CError(ERROR_FILE_NOT_FOUND, CRITICAL, "Failed to open file: " + filename);
    }

    if (result.bIsSuccess()) {
        ofs << "Success" << std::endl;
        CTree* tree = result.cGetValue();
        if (tree) {
            std::vector<std::string> nodeValues;
            tree->getTreeNodeValues(nodeValues);
            for (const auto& value : nodeValues) {
                ofs << value << std::endl;
            }
        }
        else {
            CError* error = new CError(ERROR_NULL_REFERENCE, WARNING, "Error: Tree pointer is null.");
            serializeFailures({ error }, ofs);
        }
    }
    else {
        serializeFailures(result.vGetErrors(), ofs);
    }

    ofs.close();
}

#endif
