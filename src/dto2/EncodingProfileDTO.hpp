// EncodingProfileDTO.hpp
#ifndef EncodingProfileDTO_hpp
#define EncodingProfileDTO_hpp

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for an encoding profile.
     */
    class EncodingProfileDTO final : public oatpp::DTO {
        DTO_INIT(EncodingProfileDTO, DTO)

        DTO_FIELD("profile_name", oatpp::String);
        DTO_FIELD("common_settings_id", oatpp::String);
        DTO_FIELD("filter_chain", oatpp::String);
        DTO_FIELD("bitrate_profile", oatpp::String);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* EncodingProfileDTO_hpp */