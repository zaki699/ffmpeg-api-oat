// OutputMappingDTO.hpp
#ifndef OutputMappingDTO_hpp
#define OutputMappingDTO_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * @brief Data Transfer Object for individual output mappings.
 */
class OutputMappingDTO final : public oatpp::DTO {
    DTO_INIT(OutputMappingDTO, DTO)

    /**
     * @brief The resolution identifier (e.g., "480p", "720p").
     * @example "480p"
     */
    DTO_FIELD(String, resolution, "resolution");

    /**
     * @brief The corresponding filter output (e.g., "[final_fps_480]").
     * @example "[final_fps_480]"
     */
    DTO_FIELD(String, output, "output");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* OutputMappingDTO_hpp */