// ValidationUtils.cpp

#include "oatpp/web/protocol/http/Http.hpp"
#include <unordered_set>
#include <regex>
#include <cstring> // For std::strlen

#include "dto/EncodingDTO.hpp"
#include "dto/EncodingTemplateDTO.hpp"
#include "dto/SettingsDTO.hpp"

using Status = oatpp::web::protocol::http::Status;

/**
 * @brief Exception class for validation errors.
 */
class ValidationException final : public std::runtime_error {
public:
  explicit ValidationException(const std::string& message)
    : std::runtime_error(message) {}
};

/**
 * @brief Validates an EncodingProfileDTO object.
 *
 * @param profile The encoding profile to validate.
 * @throws ValidationException if any validation rule is violated.
 */
namespace {
  void validateEncodingProfile(const oatpp::Object<EncodingDTO>& profile) {
    // Validate required fields
    if (!profile->output || std::strlen(profile->output->c_str()) == 0) {
      throw ValidationException("Field 'output' is required.");
    }

    // At least one codec should be specified
    if (!profile->video_codec && !profile->audio_codec && !profile->subtitle_codec) {
      throw ValidationException("At least one of 'video_codec', 'audio_codec', or 'subtitle_codec' must be specified.");
    }

    // Validate video codec if specified
    if (profile->video_codec) {
      static const std::unordered_set<std::string> validVideoCodecs = {
        "libx264", "libx265", "h264_nvenc", "hevc_nvenc", "h264_qsv", "hevc_qsv",
        "libvpx", "libvpx-vp9", "av1", "av1_qsv", "h264", "mpeg4"
      };
      if (const auto codec = profile->video_codec->c_str(); validVideoCodecs.find(codec) == validVideoCodecs.end()) {
        throw ValidationException("Invalid 'video_codec': " + std::string(codec));
      }

      // Validate preset if specified
      if (profile->preset) {
        static const std::unordered_set<std::string> validPresets = {
          "ultrafast", "superfast", "veryfast", "faster", "fast", "medium",
          "slow", "slower", "veryslow"
        };
        if (const auto preset = profile->preset->c_str(); validPresets.find(preset) == validPresets.end()) {
          throw ValidationException("Invalid 'preset' for video codec: " + std::string(preset));
        }
      }

      // Validate pixel format if specified
      if (profile->pix_fmt) {
        static const std::unordered_set<std::string> validPixFmts = {
          "yuv420p", "yuv422p", "yuv444p", "nv12", "p010le", "p016le"
        };
        if (const auto pixFmt = profile->pix_fmt->c_str(); validPixFmts.find(pixFmt) == validPixFmts.end()) {
          throw ValidationException("Invalid 'pix_fmt': " + std::string(pixFmt));
        }
      }

      // Validate profile if specified
      if (profile->profile) {
        const auto profileStr = profile->profile->c_str(); // Convert to const char*
        static const std::unordered_set<std::string> validProfiles = {
          "baseline", "main", "high", "high10", "high422", "high444"
        };
        if (validProfiles.find(profileStr) == validProfiles.end()) {
          throw ValidationException("Invalid 'profile' for video codec: " + std::string(profileStr));
        }
      }

      // Validate level if specified
      if (profile->level) {
        const auto levelStr = profile->level->c_str(); // Convert to const char*
        try {
          if (const float levelValue = std::stof(levelStr); levelValue < 1.0f || levelValue > 6.2f) {
            throw ValidationException("Invalid 'level' for video codec: " + std::string(levelStr));
          }
        } catch (const std::exception&) {
          throw ValidationException("Invalid 'level' format: " + std::string(levelStr));
        }
      }

      // Validate color parameters if specified
      if (profile->color_primaries || profile->color_trc || profile->colorspace) {
        static const std::unordered_set<std::string> validColorPrimaries = {
          "bt709", "bt2020", "smpte170m", "smpte240m", "film"
        };
        if (profile->color_primaries) {
          if (const auto primaries = profile->color_primaries->c_str(); validColorPrimaries.find(primaries) == validColorPrimaries.end()) {
            throw ValidationException("Invalid 'color_primaries': " + std::string(primaries));
          }
        }

        static const std::unordered_set<std::string> validColorTrc = {
          "bt709", "smpte2084", "gamma22", "gamma28", "linear"
        };
        if (profile->color_trc) {
          if (const auto trc = profile->color_trc->c_str(); validColorTrc.find(trc) == validColorTrc.end()) {
            throw ValidationException("Invalid 'color_trc': " + std::string(trc));
          }
        }

        static const std::unordered_set<std::string> validColorspaces = {
          "bt709", "bt2020nc", "bt2020c", "smpte170m"
        };
        if (profile->colorspace) {
          if (const auto colorspace = profile->colorspace->c_str(); validColorspaces.find(colorspace) == validColorspaces.end()) {
            throw ValidationException("Invalid 'colorspace': " + std::string(colorspace));
          }
        }
      }
    }

    // Validate audio codec if specified
    if (profile->audio_codec) {
      static const std::unordered_set<std::string> validAudioCodecs = {
        "aac", "ac3", "eac3", "mp3", "flac", "opus", "vorbis"
      };
      if (const auto codec = profile->audio_codec->c_str(); validAudioCodecs.find(codec) == validAudioCodecs.end()) {
        throw ValidationException("Invalid 'audio_codec': " + std::string(codec));
      }

      // Validate audio bitrate if specified
      if (profile->audio_bitrate) {
        const auto bitrateStr = profile->audio_bitrate->c_str(); // Convert to const char*
        static const std::regex bitratePattern(R"(\d+(k|M))");
        if (!std::regex_match(bitrateStr, bitratePattern)) {
          throw ValidationException("Invalid 'audio_bitrate' format: " + std::string(bitrateStr));
        }
      }

      // Validate audio channels if specified
      if (profile->audio_channels) {
        if (profile->audio_channels < 1 || profile->audio_channels > 8) {
          throw ValidationException("Invalid 'audio_channels': must be between 1 and 8.");
        }
      }
    }

    // Validate rate control mode and associated settings
    if (profile->rate_control_mode) {
      const auto mode = profile->rate_control_mode->c_str(); // Convert to const char*
      static const std::unordered_set<std::string> validRateControlModes = {
        "cbr", "vbr", "constqp", "crf", "icq"
      };
      if (validRateControlModes.find(mode) == validRateControlModes.end()) {
        throw ValidationException("Invalid 'rate_control_mode': " + std::string(mode));
      }

      if (const std::string modeStr(mode); modeStr == "cbr" || modeStr == "vbr") {
        if (!profile->bitrate_settings ||
            !profile->bitrate_settings->video_bitrate ||
            std::strlen(profile->bitrate_settings->video_bitrate->c_str()) == 0) {
          throw ValidationException("Field 'video_bitrate' is required when 'rate_control_mode' is '" + modeStr + "'.");
            }
        if (!profile->bitrate_settings->maxrate ||
            std::strlen(profile->bitrate_settings->maxrate->c_str()) == 0) {
          throw ValidationException("Field 'maxrate' is required when 'rate_control_mode' is '" + modeStr + "'.");
            }
        if (!profile->bitrate_settings->bufsize ||
            std::strlen(profile->bitrate_settings->bufsize->c_str()) == 0) {
          throw ValidationException("Field 'bufsize' is required when 'rate_control_mode' is '" + modeStr + "'.");
            }
      } else if (modeStr == "constqp") {
        if (!profile->bitrate_settings ||
            !profile->bitrate_settings->qp ||
            profile->bitrate_settings->qp == 0.0f) {
          throw ValidationException("Field 'qp' is required when 'rate_control_mode' is 'constqp'.");
            }
      } else if (modeStr == "crf") {
        if (!profile->bitrate_settings ||
            !profile->bitrate_settings->crf ||
            profile->bitrate_settings->crf == 0.0f) {
          throw ValidationException("Field 'crf' is required when 'rate_control_mode' is 'crf'.");
            }
        if (profile->bitrate_settings->crf < 0.0f || profile->bitrate_settings->crf > 51.0f) {
          throw ValidationException("Field 'crf' must be between 0 and 51.");
        }
      } else if (modeStr == "icq") {
        if (!profile->bitrate_settings ||
            !profile->bitrate_settings->global_quality ||
            profile->bitrate_settings->global_quality == 0.0f) {
          throw ValidationException("Field 'global_quality' is required when 'rate_control_mode' is 'icq'.");
            }
        if (profile->bitrate_settings->global_quality < 1.0f ||
            profile->bitrate_settings->global_quality > 31.0f) {
          throw ValidationException("Field 'global_quality' must be between 1 and 31.");
            }
      }
    }

    // Validate bitrate settings if specified
    if (profile->bitrate_settings) {
      // Validate 'video_bitrate'
      if (profile->bitrate_settings->video_bitrate) {
        const auto bitrateStr = profile->bitrate_settings->video_bitrate->c_str(); // Convert to const char*
        static const std::regex bitratePattern(R"(\d+(k|M))");
        if (!std::regex_match(bitrateStr, bitratePattern)) {
          throw ValidationException("Invalid 'video_bitrate' format: " + std::string(bitrateStr));
        }
      }

      // Validate 'maxrate'
      if (profile->bitrate_settings->maxrate) {
        const auto maxrateStr = profile->bitrate_settings->maxrate->c_str(); // Convert to const char*
        static const std::regex maxratePattern(R"(\d+(k|M))");
        if (!std::regex_match(maxrateStr, maxratePattern)) {
          throw ValidationException("Invalid 'maxrate' format: " + std::string(maxrateStr));
        }
      }

      // Validate 'bufsize'
      if (profile->bitrate_settings->bufsize) {
        const auto bufsizeStr = profile->bitrate_settings->bufsize->c_str(); // Convert to const char*
        static const std::regex bufsizePattern(R"(\d+(k|M))");
        if (!std::regex_match(bufsizeStr, bufsizePattern)) {
          throw ValidationException("Invalid 'bufsize' format: " + std::string(bufsizeStr));
        }
      }

      // Validate 'qp'
      if (profile->bitrate_settings->qp) {
        if (profile->bitrate_settings->qp < 0.0f || profile->bitrate_settings->qp > 51.0f) {
          throw ValidationException("Field 'qp' must be between 0 and 51.");
        }
      }

      // Validate 'cq'
      if (profile->bitrate_settings->cq) {
        if (profile->bitrate_settings->cq < 0.0f || profile->bitrate_settings->cq > 51.0f) {
          throw ValidationException("Field 'cq' must be between 0 and 51.");
        }
      }

      // Validate 'global_quality'
      if (profile->bitrate_settings->global_quality) {
        if (profile->bitrate_settings->global_quality < 1.0f ||
            profile->bitrate_settings->global_quality > 31.0f) {
          throw ValidationException("Field 'global_quality' must be between 1 and 31.");
            }
      }
    }

    // Validate 'maps' if specified
    if (profile->maps && !profile->maps->empty()) {
      for (const auto& map : *profile->maps) {
        if (!map || std::strlen(map->c_str()) == 0) {
          throw ValidationException("Invalid 'map' entry: empty string.");
        }
      }
    }

    // Validate 'video_filter' if specified
    if (profile->video_filter) {
      if (const auto filterStr = profile->video_filter->c_str(); std::strlen(filterStr) == 0) {
        throw ValidationException("Field 'video_filter' cannot be empty.");
      }
      // Advanced filter validation can be added here
    }

    // Validate 'audio_filter' if specified
    if (profile->audio_filter) {
      if (const auto filterStr = profile->audio_filter->c_str(); std::strlen(filterStr) == 0) {
        throw ValidationException("Field 'audio_filter' cannot be empty.");
      }
    }

    // Validate advanced settings if specified
    if (profile->advanced_settings) {
      // Validate 'rc_lookahead'
      if (profile->advanced_settings->rc_lookahead) {
        if (profile->advanced_settings->rc_lookahead < 0 ||
            profile->advanced_settings->rc_lookahead > 250) {
          throw ValidationException("Field 'rc_lookahead' must be between 0 and 250.");
            }
      }

      // Validate 'aq_strength'
      if (profile->advanced_settings->aq_strength) {
        if (profile->advanced_settings->aq_strength < 0 ||
            profile->advanced_settings->aq_strength > 15) {
          throw ValidationException("Field 'aq_strength' must be between 0 and 15.");
            }
      }

      // Additional advanced settings validations can be added here
    }

    // Validate 'subtitle_codec' if specified
    if (profile->subtitle_codec) {
      static const std::unordered_set<std::string> validSubtitleCodecs = {
        "mov_text", "ass", "srt", "webvtt", "dvd_subtitle"
      };
      if (const auto codec = profile->subtitle_codec->c_str(); validSubtitleCodecs.find(codec) == validSubtitleCodecs.end()) {
        throw ValidationException("Invalid 'subtitle_codec': " + std::string(codec));
      }

      // Validate 'subtitle_language' if specified
      if (profile->subtitle_language) {
        if (const auto lang = profile->subtitle_language->c_str(); std::strlen(lang) != 3) {
          throw ValidationException("Invalid 'subtitle_language': must be a 3-letter language code.");
        }
      }
    }

    // Validate 'audio_language' if specified
    if (profile->audio_language) {
      if (const auto lang = profile->audio_language->c_str(); std::strlen(lang) != 3) {
        throw ValidationException("Invalid 'audio_language': must be a 3-letter language code.");
      }
    }

    // Additional validations can be added as necessary
  }

  /**
   * @brief Validates a SettingsDTO object.
   *
   * @param settings The settings object to validate.
   * @throws ValidationException if any validation rule is violated.
   */
  void validateSettings(const oatpp::Object<SettingsDTO>& settings) {
    // Validate 'input' field
    if (!settings->input || std::strlen(settings->input->c_str()) == 0) {
      throw ValidationException("Field 'input' in SettingsDTO is required.");
    }

    // Validate each EncodingProfileDTO within the 'outputs' list
    if (settings->outputs) {
      for (const auto& profile : *settings->outputs) {
        if (!profile) {
          throw ValidationException("Found null profile in 'outputs' list in SettingsDTO.");
        }
        validateEncodingProfile(profile);
      }
    }

    // Additional validations for fields like 'filter_complex' or 'hardware_acceleration' can be added.
    if (settings->filter_complex && std::strlen(settings->filter_complex->c_str()) == 0) {
      throw ValidationException("Field 'filter_complex' in SettingsDTO cannot be empty if specified.");
    }
  }
}

namespace Validation {
  /**
   * @brief Validates an EncodingTemplateDTO object.
   *
   * @param templateDto The encoding template to validate.
   * @throws ValidationException if any validation rule is violated.
   */
  void validateEncodingTemplate(const oatpp::Object<EncodingTemplateDTO>& templateDto) {
    // Validate 'id' field
    if (!templateDto->id || std::strlen(templateDto->id->c_str()) == 0) {
      throw ValidationException("Field 'id' in EncodingTemplateDTO is required.");
    }

    // Validate 'description' field if specified
    if (templateDto->description && std::strlen(templateDto->description->c_str()) == 0) {
      throw ValidationException("Field 'description' in EncodingTemplateDTO cannot be empty if specified.");
    }

    // Validate the nested SettingsDTO
    if (!templateDto->settings) {
      throw ValidationException("Field 'settings' in EncodingTemplateDTO is required.");
    }
    validateSettings(templateDto->settings);

    // Additional validation on 'ffmpeg_command' if applicable
    if (templateDto->ffmpeg_command && std::strlen(templateDto->ffmpeg_command->c_str()) == 0) {
      throw ValidationException("Field 'ffmpeg_command' in EncodingTemplateDTO cannot be empty if specified.");
    }
  }
}