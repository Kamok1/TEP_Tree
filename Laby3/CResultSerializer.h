//#ifndef CRESULTSERIALIZER_H
//#define CRESULTSERIALIZER_H
//
//#include <fstream>
//#include <string>
//#include <vector>
//#include <iostream>
//#include "CTree.h"
//#define FAILURE_MESSAGE "Failure"
//#define FILE_NOT_FOUND_MESSAGE "File not found: "
//#define NULL_TREE_MESSAGE "Error: Tree pointer is null."
//#define SUCCESS_MESSAGE "Success"
//
//
//template <typename T>
//class CResultSerializer {
//public:
//    static CResult<void, CError> serialize(const CResult<T, CError>& result, const std::string& filename);
//
//private:
//    static void serializeFailures(const std::vector<CError*>& errors, std::ofstream& ofs);
//};
//
//template <typename T>
//void CResultSerializer<T>::serializeFailures(const std::vector<CError*>& errors, std::ofstream& ofs) {
//    ofs << FAILURE_MESSAGE << std::endl;
//    for (const auto& error : errors) {
//        ofs << error->toString() << std::endl;
//    }
//}
//
//template <typename T>
//CResult<void, CError> CResultSerializer<T>::serialize(const CResult<T, CError>& result, const std::string& filename) {
//    std::ofstream ofs(filename);
//    if (!ofs) {
//        return new CError(ERROR_FILE_NOT_FOUND, CRITICAL, FILE_NOT_FOUND_MESSAGE + filename);
//    }
//
//    if (result.bIsSuccess()) {
//        ofs << SUCCESS_MESSAGE << std::endl;
//        ofs << result.cGetValue().toString() << std::endl;
//    }
//    else {
//        serializeFailures(result.vGetErrors(), ofs);
//    }
//
//    ofs.close();
//}
//
//template <>
//CResult<void, CError> CResultSerializer<CTree*>::serialize(const CResult<CTree*, CError>& result, const std::string& filename) {
//    std::ofstream ofs(filename);
//    if (!ofs) {
//        return new CError(ERROR_FILE_NOT_FOUND, CRITICAL, FILE_NOT_FOUND_MESSAGE + filename);
//    }
//
//    if (result.bIsSuccess()) {
//        ofs << SUCCESS_MESSAGE << std::endl;
//        CTree* tree = result.cGetValue();
//        if (tree) {
//            std::vector<std::string> nodeValues;
//            tree->getTreeNodeValues(nodeValues);
//            for (int i = 0; i < nodeValues.size(); i++)
//            {
//                ofs << nodeValues[i] << std::endl;
//            }
//        }
//        else {
//            CError* error = new CError(ERROR_NULL_REFERENCE, WARNING, NULL_TREE_MESSAGE);
//            serializeFailures({ error }, ofs);
//        }
//    }
//    else {
//        serializeFailures(result.vGetErrors(), ofs);
//    }
//
//    ofs.close();
//}
//
//#endif
