#ifndef MEDIAINFO_METADATA_PROVIDER_HPP
#define MEDIAINFO_METADATA_PROVIDER_HPP

#include "MediaMetadataProvider.hpp"
#include <json/json.h>
#include <string>

/**
 * MediaInfoMetadataProvider uses MediaInfo to retrieve metadata for a media file.
 * It implements the MediaMetadataProvider interface to standardize the metadata retrieval process.
 */
class MediaInfoMetadataProvider final : public MediaMetadataProvider {
public:
    /**
     * Retrieves metadata for the specified file using MediaInfo.
     * @param filePath The path to the media file.
     * @return JSON representation of the metadata.
     * @throws std::runtime_error if MediaInfo fails or JSON parsing fails.
     */
    Json::Value getMetadata(const std::string& filePath) override;

private:
    /**
     * Executes the MediaInfo command and returns the raw JSON output as a string.
     * @param command The MediaInfo command to execute.
     * @return The raw JSON string output from MediaInfo.
     * @throws std::runtime_error if the command execution fails.
     */
    static std::string runCommand(const std::string& command);

    /**
     * Helper function to handle errors during MediaInfo execution.
     * @param errorMessage The error message to log and throw.
     * @throws std::runtime_error with the specified error message.
     */
    static void handleError(const std::string& errorMessage);
};

#endif // MEDIAINFO_METADATA_PROVIDER_HPP