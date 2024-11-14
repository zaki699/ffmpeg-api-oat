#pragma once

#include "models/Job.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)


class JobDto final : public oatpp::DTO {
    DTO_INIT(JobDto, DTO)

    // Job ID
    DTO_FIELD(Int32, id);  // Ensure this matches jobId type in JobManager
    DTO_FIELD(String, inputFile);
    DTO_FIELD(String, outputFile);
    DTO_FIELD(Enum<JobStatus>::AsString, status);  // Use AsString for JSON-friendly output
    // Optional field for options
    DTO_FIELD(oatpp::String, options);  // Adding 'options' field as expected by JobController
};

#include OATPP_CODEGEN_END(DTO)