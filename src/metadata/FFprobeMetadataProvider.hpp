#ifndef FFPROBE_METADATA_PROVIDER_HPP
#define FFPROBE_METADATA_PROVIDER_HPP

#include "MediaMetadataProvider.hpp"
#include <json/json.h>
#include <array>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>

/**
 * FFprobeMetadataProvider uses FFprobe to retrieve metadata for a media file.
 * It implements the MediaMetadataProvider interface to standardize the metadata retrieval process.
 */
class FFprobeMetadataProvider final : public MediaMetadataProvider {
public:
    /**
     * Retrieves metadata for the specified file using FFprobe.
     * @param filePath The path to the media file.
     * @return JSON representation of the metadata.
     * @throws std::runtime_error if FFprobe fails or JSON parsing fails.
     */
    Json::Value getMetadata(const std::string& filePath) override;

private:
    /**
     * Executes the FFprobe command and returns the raw JSON output as a string.
     * @param command The FFprobe command to execute.
     * @return The raw JSON string output from FFprobe.
     * @throws std::runtime_error if the command execution fails.
     */
    static std::string runCommand(const std::string& command);

    /**
     * Helper function to handle errors during FFprobe execution.
     * @param errorMessage The error message to log and throw.
     * @throws std::runtime_error with the specified error message.
     */
    static void handleError(const std::string& errorMessage) {
        throw std::runtime_error(errorMessage);
    }
};

#endif // FFPROBE_METADATA_PROVIDER_HPP