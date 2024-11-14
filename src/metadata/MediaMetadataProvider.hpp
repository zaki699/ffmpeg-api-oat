#ifndef MEDIA_METADATA_PROVIDER_HPP
#define MEDIA_METADATA_PROVIDER_HPP

#include <json/json.h>
#include <string>

class MediaMetadataProvider {
public:
    virtual ~MediaMetadataProvider() = default;

    // Method to retrieve metadata as a JSON object
    virtual Json::Value getMetadata(const std::string& filePath) = 0;
};

#endif // MEDIA_METADATA_PROVIDER_HPP