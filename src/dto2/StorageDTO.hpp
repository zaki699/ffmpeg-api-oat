// StorageDTO.hpp
#ifndef StorageDTO_hpp
#define StorageDTO_hpp

#include "StorageProviderDTO.hpp"
#include "CredentialsDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for storage configuration.
     */
    class StorageDTO final : public oatpp::DTO {
        DTO_INIT(StorageDTO, DTO)

        DTO_FIELD("input", oatpp::Object<StorageProviderDTO>);
        DTO_FIELD("output", oatpp::Object<StorageProviderDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* StorageDTO_hpp */