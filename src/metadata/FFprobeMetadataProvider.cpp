#include "FFprobeMetadataProvider.hpp"

Json::Value FFprobeMetadataProvider::getMetadata(const std::string& filePath) {
    std::string command = "ffprobe -v quiet -print_format json -show_format -show_streams \"" + filePath + "\"";
    std::string result = runCommand(command);

    Json::Value metadata;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;

    if (std::istringstream resultStream(result); !Json::parseFromStream(readerBuilder, resultStream, &metadata, &errs)) {
        handleError("FFprobe JSON parsing failed: " + errs);
    }

    return metadata;
}

std::string FFprobeMetadataProvider::runCommand(const std::string& command) {
    std::string result;

    // Open the pipe using popen and wrap it with unique_ptr for automatic closure
    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        handleError("Failed to run FFprobe command");
        return result;  // Return an empty result if the command fails
    }

    // Read output from the command
    while (true) {
        // Define buffer within the loop to avoid any potential memory escape warnings
        char buffer[128];

        // Read data from the pipe into the buffer
        if (fgets(buffer, sizeof(buffer), pipe.get()) == nullptr) {
            break;  // Stop reading if no more data
        }

        // Append buffer content to the result
        result += buffer;
    }

    return result;  // Return the accumulated result
}