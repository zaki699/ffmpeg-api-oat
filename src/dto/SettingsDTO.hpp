#ifndef SettingsDTO_hpp
#define SettingsDTO_hpp

#include "EncodingDTO.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* @brief Data Transfer Object for general encoding settings.
 *
 * Encapsulates the general settings for the encoding process.
 */
class SettingsDTO final : public oatpp::DTO {
    DTO_INIT(SettingsDTO, DTO)

    /**
     * @brief Input file path.
     * @example "input.mp4"
     */
    DTO_FIELD(String, input, "input");

    /**
     * @brief List of encoding profiles.
     *
     * Each profile defines an output configuration.
     */
    DTO_FIELD(List<Object<EncodingDTO>>, outputs, "outputs");

    /**
     * @brief Complex filter graph for advanced video processing.
     * @example "[0:v]split=2[v1][v2];[v1]scale=1920:1080[v1out];[v2]scale=1280:720[v2out]"
     */
    DTO_FIELD(String, filter_complex, "filter_complex");

    /**
     * @brief Hardware acceleration to use.
     * @example "cuda"
     */
    DTO_FIELD(String, hardware_acceleration, "hardware_acceleration");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* SettingsDTO_hpp */
