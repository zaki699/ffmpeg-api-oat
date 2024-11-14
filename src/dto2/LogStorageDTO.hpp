// LogStorageDTO.hpp
#ifndef LogStorageDTO_hpp
#define LogStorageDTO_hpp

#include "CredentialsDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for log storage configuration.
     */
    class LogStorageDTO final : public oatpp::DTO {
        DTO_INIT(LogStorageDTO, DTO)

        DTO_FIELD("provider", oatpp::String);
        DTO_FIELD("path", oatpp::String);
        DTO_FIELD("credentials", oatpp::Object<CredentialsDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* LogStorageDTO_hpp */