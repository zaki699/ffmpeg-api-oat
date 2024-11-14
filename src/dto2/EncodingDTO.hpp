// EncodingDTO.hpp
#ifndef EncodingDTO_hpp
#define EncodingDTO_hpp

#include "EncodingProfileDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for encoding configuration.
     */
    class EncodingDTO final : public oatpp::DTO {
        DTO_INIT(EncodingDTO, DTO)

        DTO_FIELD("profiles", std::vector<oatpp::Object<EncodingProfileDTO>>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* EncodingDTO_hpp */