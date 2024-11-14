// LoggingDTO.hpp
#ifndef LoggingDTO_hpp
#define LoggingDTO_hpp

#include "LogStorageDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for logging configuration.
     */
    class LoggingDTO final : public oatpp::DTO {
        DTO_INIT(LoggingDTO, DTO)

        DTO_FIELD("enabled", oatpp::Boolean);
        DTO_FIELD("log_level", oatpp::String);
        DTO_FIELD("log_storage", oatpp::Object<LogStorageDTO>);
        DTO_FIELD("format", oatpp::String);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* LoggingDTO_hpp */