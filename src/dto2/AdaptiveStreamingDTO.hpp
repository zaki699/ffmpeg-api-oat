// AdaptiveStreamingDTO.hpp
#ifndef AdaptiveStreamingDTO_hpp
#define AdaptiveStreamingDTO_hpp

#include "AdaptiveProfileDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for adaptive streaming configuration.
     */
    class AdaptiveStreamingDTO final : public oatpp::DTO {
        DTO_INIT(AdaptiveStreamingDTO, DTO)

        DTO_FIELD("enabled", oatpp::Boolean);
        DTO_FIELD("profiles", std::vector<oatpp::Object<AdaptiveProfileDTO>>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* AdaptiveStreamingDTO_hpp */