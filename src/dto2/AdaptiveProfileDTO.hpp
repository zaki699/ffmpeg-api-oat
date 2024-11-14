// AdaptiveProfileDTO.hpp
#ifndef AdaptiveProfileDTO_hpp
#define AdaptiveProfileDTO_hpp

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for an adaptive streaming profile.
     */
    class AdaptiveProfileDTO final : public oatpp::DTO {
        DTO_INIT(AdaptiveProfileDTO, DTO)

        DTO_FIELD("resolution", oatpp::String);
        DTO_FIELD("bitrate", oatpp::String);
        DTO_FIELD("maxrate", oatpp::String);
        DTO_FIELD("bufsize", oatpp::String);
        DTO_FIELD("encoding_profile", oatpp::String);
        DTO_FIELD("filter_chain", oatpp::String);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* AdaptiveProfileDTO_hpp */