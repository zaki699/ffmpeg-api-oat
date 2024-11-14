#include "MediaInfoMetadataProvider.hpp"
#include <array>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <cstdio> // for popen and pclose

Json::Value MediaInfoMetadataProvider::getMetadata(const std::string& filePath) {
    std::string command = "mediainfo --Output=JSON \"" + filePath + "\"";
    std::string result = runCommand(command);

    Json::Value metadata;
    std::string errs;

    std::istringstream resultStream(result);
    if (Json::CharReaderBuilder readerBuilder; !Json::parseFromStream(readerBuilder, resultStream, &metadata, &errs)) {
        throw std::runtime_error("MediaInfo JSON parsing failed: " + errs);
    }

    return metadata;
}

std::string MediaInfoMetadataProvider::runCommand(const std::string& command) {
    std::string result;
    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        handleError("Failed to run MediaInfo command");
    }

    while (true) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe.get()) == nullptr) break;
        result += buffer;
    }

    return result;
}

void MediaInfoMetadataProvider::handleError(const std::string& errorMessage) {
    throw std::runtime_error(errorMessage);
}