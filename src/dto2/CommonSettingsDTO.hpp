// CommonSettingsDTO.hpp
#ifndef CommonSettingsDTO_hpp
#define CommonSettingsDTO_hpp

#include "VideoSettingsDTO.hpp"
#include "AudioSettingsDTO.hpp"
#include "SubtitlesSettingsDTO.hpp"
#include "DRMSettingsDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for common encoding settings.
     */
    class CommonSettingsDTO final : public oatpp::DTO {
        DTO_INIT(CommonSettingsDTO, DTO)

        DTO_FIELD("id", oatpp::String);
        DTO_FIELD("video", oatpp::Object<VideoSettingsDTO>);
        DTO_FIELD("audio", oatpp::Object<AudioSettingsDTO>);
        DTO_FIELD("subtitles", oatpp::Object<SubtitlesSettingsDTO>);
        DTO_FIELD("drm", oatpp::Object<DRMSettingsDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* CommonSettingsDTO_hpp */