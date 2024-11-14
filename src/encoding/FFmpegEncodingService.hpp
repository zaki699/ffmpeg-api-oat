#pragma once

#include "interfaces/IEncodingService.hpp"
#include <string>
#include <vector>

/**
 * FFmpegEncodingService provides functionality to encode media files using FFmpeg.
 * Implements the IEncodingService interface.
 */
class FFmpegEncodingService final : public IEncodingService {
public:
    /**
     * Encodes a media file using FFmpeg with specified input and output paths and encoding options.
     * @param inputFilePath - The path to the input file to be encoded.
     * @param outputFilePath - The path where the encoded output file should be saved.
     * @param options - Additional options for FFmpeg (e.g., encoding settings).
     * @return true if encoding was successful, false otherwise.
     */
    bool encode(const std::string& inputFilePath, const std::string& outputFilePath, const std::vector<std::string>& options) override;
};