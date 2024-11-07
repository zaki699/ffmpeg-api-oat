#pragma once
#include <string>

class IStorageProvider {
public:
    virtual ~IStorageProvider() = default;

    virtual bool uploadFile(const std::string& localPath, const std::string& remotePath) = 0;
    virtual bool deleteFile(const std::string& remotePath) = 0;
    virtual std::string getURL(const std::string& remotePath) = 0;
};