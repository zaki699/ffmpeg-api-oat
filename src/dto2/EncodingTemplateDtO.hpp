#ifndef EncodingTemplateDTO_hpp
#define EncodingTemplateDTO_hpp

#include "CommonSettingsDTO.hpp"
#include "StorageDTO.hpp"
#include "InputDTO.hpp"
#include "FilterComplexDTO.hpp"
#include "EncodingDTO.hpp"
#include "AdaptiveStreamingDTO.hpp"
#include "OutputDTO.hpp"
#include "PackagerDTO.hpp"
#include "LoggingDTO.hpp"
#include "ThumbnailGenerationDTO.hpp"
#include "PreProcessingDTO.hpp"
#include "PostProcessingDTO.hpp"
#include "CdnDistributionDTO.hpp"
#include "ServiceSettingsDTO.hpp"
#include "CommonVariablesDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for the entire application configuration.
     */
    class EncodingTemplateDTO final : public oatpp::DTO {
        DTO_INIT(EncodingTemplateDTO, DTO)

        DTO_FIELD("common_settings", std::vector<oatpp::Object<CommonSettingsDTO>>);
        DTO_FIELD("storage", oatpp::Object<StorageDTO>);
        DTO_FIELD("input", oatpp::Object<InputDTO>);
        DTO_FIELD("filter_complex", oatpp::Object<FilterComplexDTO>);
        DTO_FIELD("encoding", oatpp::Object<EncodingDTO>);
        DTO_FIELD("adaptive_streaming", oatpp::Object<AdaptiveStreamingDTO>);
        DTO_FIELD("output", oatpp::Object<OutputDTO>);
        DTO_FIELD("packager", oatpp::Object<PackagerDTO>);
        DTO_FIELD("logging", oatpp::Object<LoggingDTO>);
        DTO_FIELD("thumbnail_generation", oatpp::Object<ThumbnailGenerationDTO>);
        DTO_FIELD("pre_processing", oatpp::Object<PreProcessingDTO>);
        DTO_FIELD("post_processing", oatpp::Object<PostProcessingDTO>);
        DTO_FIELD("cdn_distribution", oatpp::Object<CdnDistributionDTO>);
        DTO_FIELD("service_settings", oatpp::Object<ServiceSettingsDTO>);
        DTO_FIELD("common_variables", oatpp::Object<CommonVariablesDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* EncodingTemplateDTO_hpp */