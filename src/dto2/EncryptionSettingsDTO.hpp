// EncryptionSettingsDTO.hpp
#ifndef EncryptionSettingsDTO_hpp
#define EncryptionSettingsDTO_hpp

#include "WidevineDTO.hpp"
#include "PlayreadyDTO.hpp"
#include "FairplayDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for DRM settings.
     */
    class EncryptionSettingsDTO final : public oatpp::DTO {
        DTO_INIT(EncryptionSettingsDTO, DTO)

        DTO_FIELD("protection_scheme", oatpp::String);
        DTO_FIELD("key_server_url", oatpp::String);
        DTO_FIELD("key_id", oatpp::String);
        DTO_FIELD("content_key", oatpp::String);
        DTO_FIELD("iv", oatpp::String);
        DTO_FIELD("pssh", oatpp::String);
        DTO_FIELD("widevine", oatpp::Object<WidevineDTO>);
        DTO_FIELD("playready", oatpp::Object<PlayreadyDTO>);
        DTO_FIELD("fairplay", oatpp::Object<FairplayDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* EncryptionSettingsDTO_hpp */