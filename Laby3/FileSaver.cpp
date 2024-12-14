#include "FileSaver.h"
#include "CError.h"
#include <fstream>
#include <stdexcept>

FileSaver::FileSaver(const std::string& filePath) : filePath(filePath) {}

void FileSaver::save(const CError& error) const {
    std::ofstream file(filePath.c_str(), std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }
    file << error.toString() << std::endl;
    file.close();
}
