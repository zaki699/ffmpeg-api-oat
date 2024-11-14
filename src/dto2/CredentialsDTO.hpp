// CredentialsDTO.hpp
#ifndef CredentialsDTO_hpp
#define CredentialsDTO_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for storage credentials.
     */
    class CredentialsDTO final : public oatpp::DTO {
        DTO_INIT(CredentialsDTO, DTO)

        DTO_FIELD("secret_id", oatpp::String);
        // Add other credential fields if necessary
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* CredentialsDTO_hpp */