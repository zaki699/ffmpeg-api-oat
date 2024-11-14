// FilterComplexDTO.hpp
#ifndef FilterComplexDTO_hpp
#define FilterComplexDTO_hpp

#include "FilterChainDTO.hpp"
#include "FinalOutputDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for filter complex configuration.
     */
    class FilterComplexDTO final : public oatpp::DTO {
        DTO_INIT(FilterComplexDTO, DTO)

        DTO_FIELD("chains", std::vector<oatpp::Object<FilterChainDTO>>);
        DTO_FIELD("final_output", oatpp::Object<FinalOutputDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* FilterComplexDTO_hpp */