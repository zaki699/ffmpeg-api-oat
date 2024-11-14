#ifndef AdvancedSettingsDTO_hpp
#define AdvancedSettingsDTO_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* @brief Data Transfer Object for advanced encoder settings.
 *
 * Contains advanced settings for fine-tuning encoder behavior.
 */
class AdvancedSettingsDTO final : public oatpp::DTO {
    DTO_INIT(AdvancedSettingsDTO, DTO)

    /**
     * @brief Number of frames for rate control lookahead.
     * @example 32
     */
    DTO_FIELD(UInt32, rc_lookahead, "rc_lookahead");

    /**
     * @brief Enable temporal adaptive quantization.
     *
     * 0 = disabled, 1 = enabled.
     * @example 1
     */
    DTO_FIELD(UInt8, temporal_aq, "temporal_aq");

    /**
     * @brief Enable spatial adaptive quantization.
     *
     * 0 = disabled, 1 = enabled.
     * @example 1
     */
    DTO_FIELD(UInt8, spatial_aq, "spatial_aq");

    /**
     * @brief Strength of adaptive quantization.
     *
     * Range: 0-15.
     * @example 8
     */
    DTO_FIELD(UInt8, aq_strength, "aq_strength");

    /**
     * @brief Enable lookahead.
     *
     * 0 = disabled, 1 = enabled.
     * @example 1
     */
    DTO_FIELD(UInt8, look_ahead, "look_ahead");

    /**
     * @brief Depth of the lookahead in frames.
     * @example 40
     */
    DTO_FIELD(UInt32, look_ahead_depth, "look_ahead_depth");

    /**
     * @brief Additional parameters for the libx264 encoder.
     * @example "vbv-maxrate=5000:vbv-bufsize=10000"
     */
    DTO_FIELD(String, x264_params, "x264_params");

    /**
     * @brief Additional parameters for the libx265 encoder.
     * @example "vbv-maxrate=5000:vbv-bufsize=10000"
     */
    DTO_FIELD(String, x265_params, "x265_params");

    /**
     * @brief Other encoder-specific parameters.
     */
    DTO_FIELD(String, other_params, "other_params");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* AdvancedSettingsDTO_hpp */