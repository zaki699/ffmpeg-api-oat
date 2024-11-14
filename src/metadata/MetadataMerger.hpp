#ifndef METADATA_MERGER_HPP
#define METADATA_MERGER_HPP

#include "FFprobeMetadataProvider.hpp"
#include "MediaInfoMetadataProvider.hpp"
#include <json/json.h>

/**
 * MetadataMerger is a utility class for retrieving and merging metadata from multiple providers.
 * It uses both FFprobe and MediaInfo providers to retrieve metadata and then merges them,
 * allowing for more complete metadata coverage.
 */
class MetadataMerger {
public:
    /**
     * Constructor for MetadataMerger, initializing with specific metadata providers.
     */
    MetadataMerger();

    /**
     * Retrieves and merges metadata for the specified file by calling both FFprobe and MediaInfo.
     * Any missing fields in one provider's metadata will be filled by the other.
     * @param filePath The path to the media file.
     * @return The merged JSON metadata object.
     * @throws std::runtime_error if both providers fail to retrieve metadata.
     */
    Json::Value getMergedMetadata(const std::string& filePath) const;

private:
    std::unique_ptr<FFprobeMetadataProvider> ffprobeProvider;
    std::unique_ptr<MediaInfoMetadataProvider> mediaInfoProvider;

    /**
     * Helper function to merge two JSON objects for specific stream or format data.
     * Fields from the secondary object will only be added if they are missing or null in the primary object.
     * @param primaryStream The primary JSON object for a specific stream or format.
     * @param secondaryStream The secondary JSON object for a specific stream or format.
     * @return The merged JSON object for the specific stream or format.
     */
    static Json::Value mergeStreamData(const Json::Value& primaryStream, const Json::Value& secondaryStream);
};

#endif // METADATA_MERGER_HPP