// DimensionsDTO.hpp
#ifndef DimensionsDTO_hpp
#define DimensionsDTO_hpp

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for dimensions.
     */
    class DimensionsDTO final : public oatpp::DTO {
        DTO_INIT(DimensionsDTO, DTO)

        DTO_FIELD("width", oatpp::Int32);
        DTO_FIELD("height", oatpp::Int32);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* DimensionsDTO_hpp */