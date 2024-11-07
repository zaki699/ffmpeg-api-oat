#pragma once
#include <string>
#include <vector>

class IEncodingService {
public:
    virtual ~IEncodingService() = default;

    virtual bool encode(const std::string& inputFilePath, const std::string& outputFilePath, const std::vector<std::string>& options) = 0;
};