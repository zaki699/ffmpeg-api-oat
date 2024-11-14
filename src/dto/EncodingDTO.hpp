#ifndef EncodingDTO_hpp
#define EncodingDTO_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "dto/AdvancedSettingsDTO.hpp"
#include "dto/BitrateSettingsDTO.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)



/**
 * @brief Data Transfer Object for encoding profile.
 *
 * Represents the encoding configuration for a single output file.
 */
class EncodingDTO final : public oatpp::DTO {
  DTO_INIT(EncodingDTO, DTO)

  /**
   * @brief Output file path.
   * @example "output_1080p.mp4"
   */
  DTO_FIELD(String, output, "output");

  /**
   * @brief Stream mappings.
   *
   * Specifies the input streams to map to the output.
   * @example ["[v0]", "0:a"]
   */
  DTO_FIELD(List<String>, maps, "maps");

  /**
   * @brief Video codec to use for encoding.
   * @example "libx264"
   */
  DTO_FIELD(String, video_codec, "video_codec");

  /**
   * @brief Audio codec to use for encoding.
   * @example "aac"
   */
  DTO_FIELD(String, audio_codec, "audio_codec");

  /**
   * @brief Subtitle codec to use for encoding.
   * @example "mov_text"
   */
  DTO_FIELD(String, subtitle_codec, "subtitle_codec");

  /**
   * @brief Encoder preset.
   *
   * Adjusts encoding speed vs. compression efficiency.
   * @example "fast"
   */
  DTO_FIELD(String, preset, "preset");

  /**
   * @brief Rate control mode.
   *
   * Determines how the encoder controls bitrate.
   * @example "cbr"
   */
  DTO_FIELD(String, rate_control_mode, "rate_control_mode");

  /**
   * @brief Bitrate and quality settings.
   *
   * An object containing bitrate-related parameters.
   */
  DTO_FIELD(Object<BitrateSettingsDTO>, bitrate_settings, "bitrate_settings");

  /**
   * @brief Advanced encoder settings.
   *
   * An object containing advanced parameters for the encoder.
   */
  DTO_FIELD(Object<AdvancedSettingsDTO>, advanced_settings, "advanced_settings");

  /**
   * @brief Video filters to apply.
   * @example "scale=1280:720"
   */
  DTO_FIELD(String, video_filter, "video_filter");

  /**
   * @brief Audio filters to apply.
   * @example "volume=1.5"
   */
  DTO_FIELD(String, audio_filter, "audio_filter");

  /**
   * @brief Subtitle filters to apply.
   */
  DTO_FIELD(String, subtitle_filter, "subtitle_filter");

  /**
   * @brief Pixel format of the output video.
   * @example "yuv420p"
   */
  DTO_FIELD(String, pix_fmt, "pix_fmt");

  /**
   * @brief Codec profile.
   * @example "high"
   */
  DTO_FIELD(String, profile, "profile");

  /**
   * @brief Codec level.
   * @example "4.1"
   */
  DTO_FIELD(String, level, "level");

  /**
   * @brief Color primaries.
   * @example "bt709"
   */
  DTO_FIELD(String, color_primaries, "color_primaries");

  /**
   * @brief Color transfer characteristics.
   * @example "smpte2084"
   */
  DTO_FIELD(String, color_trc, "color_trc");

  /**
   * @brief Color space.
   * @example "bt709"
   */
  DTO_FIELD(String, colorspace, "colorspace");

  /**
   * @brief Number of audio channels.
   * @example 2
   */
  DTO_FIELD(UInt8, audio_channels, "audio_channels");

  /**
   * @brief Audio bitrate.
   * @example "192k"
   */
  DTO_FIELD(String, audio_bitrate, "audio_bitrate");

  /**
   * @brief Language code for the audio track.
   * @example "eng"
   */
  DTO_FIELD(String, audio_language, "audio_language");

  /**
   * @brief Language code for the subtitle track.
   * @example "eng"
   */
  DTO_FIELD(String, subtitle_language, "subtitle_language");
};


#include OATPP_CODEGEN_END(DTO)

#endif /* EncodingDTO_hpp */