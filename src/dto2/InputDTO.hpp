// InputDTO.hpp
#ifndef InputDTO_hpp
#define InputDTO_hpp

#include "InputOptionsDTO.hpp"

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for input configuration.
     */
    class InputDTO final : public oatpp::DTO {
        DTO_INIT(InputDTO, DTO)

        DTO_FIELD("source", oatpp::String);
        DTO_FIELD("start_time", oatpp::Int32);
        DTO_FIELD("duration", oatpp::Float32); // Use Nullable if duration can be null
        DTO_FIELD("options", oatpp::Object<InputOptionsDTO>);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* InputDTO_hpp */