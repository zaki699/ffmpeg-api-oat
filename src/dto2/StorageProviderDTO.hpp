// StorageProviderDTO.hpp
#ifndef StorageProviderDTO_hpp
#define StorageProviderDTO_hpp

#include "CredentialsDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for storage provider configuration.
     */
    class StorageProviderDTO final : public oatpp::DTO {
        DTO_INIT(StorageProviderDTO, DTO)

        DTO_FIELD("provider", oatpp::String);
        DTO_FIELD("path", oatpp::String);          // For 'input' provider
        DTO_FIELD("base_path", oatpp::String);     // For 'output' provider
        DTO_FIELD("credentials", oatpp::Object<CredentialsDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* StorageProviderDTO_hpp */