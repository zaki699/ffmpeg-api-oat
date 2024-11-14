#pragma once

#include "managers/JobManager.hpp"
#include "dto/JobDto.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin code-generation region

class JobController final : public oatpp::web::server::api::ApiController {
public:
    explicit JobController(const std::shared_ptr<JobManager>& jobManager, const std::shared_ptr<ObjectMapper>& objectMapper)
        : ApiController(objectMapper)
        , m_jobManager(jobManager)
    {}

private:
    std::shared_ptr<JobManager> m_jobManager;

    /**
     * @brief Endpoint to create a new job.
     *
     * Accepts a `JobDto` JSON payload and creates a job using `JobManager`.
     *
     * @param dto - Parsed `JobDto` from the incoming request body.
     * @return `oatpp::web::protocol::http::outgoing::Response` indicating success or failure.
     */
    ENDPOINT("POST", "/jobs", createJob,
             BODY_DTO(oatpp::Object<JobDto>, dto)) {

        // Inject JobManager component to handle job logic.
        OATPP_COMPONENT(std::shared_ptr<JobManager>, jobManager);

        if (!dto) {
            return createResponse(Status::CODE_400, "Invalid Job Data");
        }

        // Attempt to create the job using the JobManager.

        if (int jobId = jobManager->createJob(dto->inputFile, dto->outputFile, dto->options); jobId > 0) {
            return createResponse(Status::CODE_201, "Job created with ID: " + std::to_string(jobId));
        } else {
            return createResponse(Status::CODE_500, "Failed to create job");
        }
    }
};

#include OATPP_CODEGEN_END(ApiController) ///< End code-generation region