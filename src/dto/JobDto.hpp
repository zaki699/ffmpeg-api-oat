#pragma once

#include "Job.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)


class JobDto : public oatpp::DTO {
    DTO_INIT(JobDto, DTO)

    DTO_FIELD(String, id);
    DTO_FIELD(String, inputFile);
    DTO_FIELD(String, outputFile);
    DTO_FIELD(Enum<JobStatus>::AsString, status);  // Use AsString for JSON-friendly output
};

#include OATPP_CODEGEN_END(DTO)