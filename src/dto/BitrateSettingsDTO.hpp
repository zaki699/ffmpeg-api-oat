#ifndef BitrateSettingsDTO_hpp
#define BitrateSettingsDTO_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)
/**
* @brief Data Transfer Object for bitrate settings.
 *
 * Contains settings related to bitrate and quality for video encoding.
 */
class BitrateSettingsDTO final : public oatpp::DTO {
    DTO_INIT(BitrateSettingsDTO, DTO)

    /**
     * @brief Target video bitrate.
     * @example "5M" for 5 megabits per second.
     */
    DTO_FIELD(String, video_bitrate, "video_bitrate");

    /**
     * @brief Maximum video bitrate.
     * @example "5M".
     */
    DTO_FIELD(String, maxrate, "maxrate");

    /**
     * @brief Minimum video bitrate.
     * @example "3M".
     */
    DTO_FIELD(String, minrate, "minrate");

    /**
     * @brief Buffer size for rate control.
     * @example "10M".
     */
    DTO_FIELD(String, bufsize, "bufsize");

    /**
     * @brief Constant Rate Factor.
     *
     * Controls the output quality for encoders like libx264 and libx265.
     * Lower values mean better quality.
     * @example 23.0
     */
    DTO_FIELD(Float32, crf, "crf");

    /**
     * @brief Quantization Parameter.
     *
     * Controls the compression level; lower values result in better quality.
     * @example 23
     */
    DTO_FIELD(Float32, qp, "qp");

    /**
     * @brief Constant QP value.
     *
     * Used in hardware encoders like NVIDIA NVENC.
     * @example 23
     */
    DTO_FIELD(Float32, cq, "cq");

    /**
     * @brief Global quality setting.
     *
     * Used in some encoders (e.g., Intel QSV).
     * @example 23
     */
    DTO_FIELD(Float32, global_quality, "global_quality");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* BitrateSettingsDTO_hpp */