// ErrorHandlingDTO.hpp
#ifndef ErrorHandlingDTO_hpp
#define ErrorHandlingDTO_hpp

#include "AlertDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for error handling configuration.
     */
    class ErrorHandlingDTO final : public oatpp::DTO {
        DTO_INIT(ErrorHandlingDTO, DTO)

        DTO_FIELD("on_fail", oatpp::String);
        DTO_FIELD("max_retries", oatpp::Int32);
        DTO_FIELD("log_level", oatpp::String);
        DTO_FIELD("alert", oatpp::Object<AlertDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* ErrorHandlingDTO_hpp */