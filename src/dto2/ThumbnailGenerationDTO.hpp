// ThumbnailGenerationDTO.hpp
#ifndef ThumbnailGenerationDTO_hpp
#define ThumbnailGenerationDTO_hpp

#include "DimensionsDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for thumbnail generation settings.
     */
    class ThumbnailGenerationDTO final : public oatpp::DTO {
        DTO_INIT(ThumbnailGenerationDTO, DTO)

        DTO_FIELD("enabled", oatpp::Boolean);
        DTO_FIELD("interval", oatpp::Int32);
        DTO_FIELD("dimensions", oatpp::Object<DimensionsDTO>);
        DTO_FIELD("output_path", oatpp::String);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* ThumbnailGenerationDTO_hpp */