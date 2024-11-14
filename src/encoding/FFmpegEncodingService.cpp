#include "FFmpegEncodingService.hpp"
#include "utils/Logger.hpp"
#include "utils/ConfigManager.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <utils/logger/LoggerMacros.hpp>

/**
 * Encodes a media file using FFmpeg.
 * @param inputFilePath - the input file path to be encoded.
 * @param outputFilePath - the output file path for the encoded file.
 * @param options - a vector of additional FFmpeg options.
 * @return true if encoding was successful, false otherwise.
 */
bool FFmpegEncodingService::encode(const std::string& inputFilePath, const std::string& outputFilePath, const std::vector<std::string>& options) {
    // Retrieve default options from configuration
    const auto defaultOptions = ConfigManager::getInstance().get<std::vector<std::string>>("ffmpeg.default_options");

    // Construct the FFmpeg command
    std::ostringstream command;
    command << "ffmpeg -i " << "\"" << inputFilePath << "\" ";

    // Append default options from configuration
    for (const auto& option : defaultOptions) {
        command << option << " ";
    }

    // Append any additional options
    for (const auto& option : options) {
        command << option << " ";
    }

    // Specify the output file
    command << "\"" << outputFilePath << "\"";

    // Log the command
    LOG_INFO("Executing FFmpeg command: %s", command.str().c_str());

    // Execute the command
    if (const int result = std::system(command.str().c_str()); result == 0) {
        LOG_INFO("Encoding completed successfully for file: %s", outputFilePath.c_str());
        return true;
    } else {
        LOG_ERROR("Encoding failed for file: %s", outputFilePath.c_str());
        return false;
    }
}