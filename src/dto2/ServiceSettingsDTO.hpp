// ServiceSettingsDTO.hpp
#ifndef ServiceSettingsDTO_hpp
#define ServiceSettingsDTO_hpp

#include "ErrorHandlingDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for service settings.
     */
    class ServiceSettingsDTO final : public oatpp::DTO {
        DTO_INIT(ServiceSettingsDTO, DTO)

        DTO_FIELD("run_async", oatpp::Boolean);
        DTO_FIELD("priority", oatpp::String);
        DTO_FIELD("callback_url", oatpp::String);
        DTO_FIELD("retries", oatpp::Int32);
        DTO_FIELD("timeout", oatpp::Int32);
        DTO_FIELD("retry_delay", oatpp::Int32);
        DTO_FIELD("error_handling", oatpp::Object<ErrorHandlingDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* ServiceSettingsDTO_hpp */