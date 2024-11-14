// OutputDTO.hpp
#ifndef OutputDTO_hpp
#define OutputDTO_hpp

#include "PackageDTO.hpp"
#include "ContainerOptionsDTO.hpp"
#include "MetadataDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for output configuration.
     */
    class OutputDTO final : public oatpp::DTO {
        DTO_INIT(OutputDTO, DTO)

        DTO_FIELD("formats", std::vector<oatpp::String>);
        DTO_FIELD("base_path", oatpp::String);
        DTO_FIELD("packages", std::vector<oatpp::Object<PackageDTO>>);
        DTO_FIELD("container_options", oatpp::Object<ContainerOptionsDTO>);
        DTO_FIELD("metadata", oatpp::Object<MetadataDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* OutputDTO_hpp */