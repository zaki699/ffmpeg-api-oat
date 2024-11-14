// InputOptionsDTO.hpp
#ifndef InputOptionsDTO_hpp
#define InputOptionsDTO_hpp

#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace App::DTO {

    /**
     * @brief Data Transfer Object for input options.
     */
    class InputOptionsDTO final : public oatpp::DTO {
        DTO_INIT(InputOptionsDTO, DTO)

        DTO_FIELD("framerate", oatpp::Int32);
        DTO_FIELD("pixel_format", oatpp::String);
        DTO_FIELD("thread_count", oatpp::Int32);
        DTO_FIELD("deinterlace", oatpp::Boolean);
        DTO_FIELD("audio_stream_index", oatpp::Int32);
        DTO_FIELD("video_stream_index", oatpp::Int32);
        DTO_FIELD("hwaccel", oatpp::String);
        DTO_FIELD("fast_seek", oatpp::Boolean);
        DTO_FIELD("timestamp_correction", oatpp::Boolean);
    };

} // namespace App::DTO

#include OATPP_CODEGEN_END(DTO)

#endif /* InputOptionsDTO_hpp */