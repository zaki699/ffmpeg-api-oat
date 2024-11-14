// FilterChainDTO.hpp
#ifndef FilterChainDTO_hpp
#define FilterChainDTO_hpp

#include "ScaleParamsDTO.hpp" // Example specific params DTO
#include <oatpp/core/macro/codegen.hpp>
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for a single filter chain.
     */
    class FilterChainDTO final : public oatpp::DTO {
        DTO_INIT(FilterChainDTO, DTO)

        DTO_FIELD("type", oatpp::String);
        DTO_FIELD("params", oatpp::String); // For simplicity, using string. Alternatively, use a more structured DTO.
        DTO_FIELD("inputs", std::vector<oatpp::String>);
        DTO_FIELD("outputs", oatpp::String);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* FilterChainDTO_hpp */