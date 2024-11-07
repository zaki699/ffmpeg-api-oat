#pragma once
#include "IEncodingService.hpp"

class FFmpegEncodingService : public IEncodingService {
public:
    bool encode(const std::string& inputFilePath, const std::string& outputFilePath, const std::vector<std::string>& options) override;
};