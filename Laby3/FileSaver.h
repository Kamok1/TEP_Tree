#ifndef FILESAVER_H
#define FILESAVER_H

#include "ISaver.h"
#include <string>

class FileSaver : public ISaver {
public:
    explicit FileSaver(const std::string& filePath);
    virtual void save(const CError& error) const override;

private:
    std::string filePath;
};

#endif
