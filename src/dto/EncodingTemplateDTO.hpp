#ifndef EncodingTemplateDTO_hpp
#define EncodingTemplateDTO_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "dto/SettingsDTO.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* @brief Data Transfer Object for the entire encoding template.
 *
 * Represents the encoding template including identification and settings.
 */
class EncodingTemplateDTO final : public oatpp::DTO {
    DTO_INIT(EncodingTemplateDTO, DTO)

    /**
     * @brief Unique identifier for the template.
     * @example "template001"
     */
    DTO_FIELD(String, id, "id");

    /**
     * @brief Description of the template.
     * @example "High-quality encoding with NVIDIA NVENC"
     */
    DTO_FIELD(String, description, "description");

    /**
     * @brief Generated ffmpeg command (optional).
     *
     * Can be used for preview or debugging purposes.
     */
    DTO_FIELD(String, ffmpeg_command, "ffmpeg_command");

    /**
     * @brief Encoding settings.
     *
     * An instance of SettingsDTO containing the encoding configurations.
     */
    DTO_FIELD(Object<SettingsDTO>, settings, "settings");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* EncodingTemplateDTO_hpp */