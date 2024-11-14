#include "MetadataMerger.hpp"
#include "utils/Logger.hpp"

MetadataMerger::MetadataMerger()
  : ffprobeProvider(std::make_unique<FFprobeMetadataProvider>()),
    mediaInfoProvider(std::make_unique<MediaInfoMetadataProvider>()) {}

Json::Value MetadataMerger::getMergedMetadata(const std::string& filePath) const {
    Json::Value ffprobeMetadata;
    Json::Value mediaInfoMetadata;

    try {
        ffprobeMetadata = ffprobeProvider->getMetadata(filePath);
    } catch (const std::exception& e) {
        Logger::getInstance().error("FFprobe failed: {}. Attempting MediaInfo.", e.what());
    }

    try {
        mediaInfoMetadata = mediaInfoProvider->getMetadata(filePath);
    } catch (const std::exception& e) {
        Logger::getInstance().error("MediaInfo failed:  {}. Using FFprobe data only.", e.what());
    }

    if (ffprobeMetadata.isNull() && mediaInfoMetadata.isNull()) {
        throw std::runtime_error("Both FFprobe and MediaInfo failed to retrieve metadata.");
    }

    return mergeStreamData(ffprobeMetadata, mediaInfoMetadata);
}

Json::Value MetadataMerger::mergeStreamData(const Json::Value& primary, const Json::Value& secondary) {
    Json::Value merged = primary;

    for (const auto& key : secondary.getMemberNames()) {
        if (!merged.isMember(key) || merged[key].isNull()) {
            merged[key] = secondary[key];
        }
    }

    return merged;
}