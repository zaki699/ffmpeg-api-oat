// PackageDTO.hpp
#ifndef PackageDTO_hpp
#define PackageDTO_hpp

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for a packaging configuration.
     */
    class PackageDTO final : public oatpp::DTO {
        DTO_INIT(PackageDTO, DTO)

        DTO_FIELD("format", oatpp::String);
        DTO_FIELD("output_path", oatpp::String);
        DTO_FIELD("playlist_name", oatpp::String);
        DTO_FIELD("segment_template", oatpp::String);
        DTO_FIELD("master_playlist_output", oatpp::String);
        DTO_FIELD("mpd_output", oatpp::String); // For DASH
        DTO_FIELD("init_segment", oatpp::String); // For DASH
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* PackageDTO_hpp */