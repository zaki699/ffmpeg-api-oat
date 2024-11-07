#pragma once

#include "JobManager.hpp"
#include "JobDto.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

class JobController : public oatpp::web::server::api::ApiController {
public:
    JobController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {}

public:
    // Asynchronous endpoint for creating a job
    ENDPOINT_ASYNC("POST", "/jobs", CreateJob) {
        ENDPOINT_ASYNC_INIT(CreateJob)

        Action act() override {
            OATPP_COMPONENT(std::shared_ptr<JobManager>, jobManager);  // Inject JobManager component
            auto jobDto = request->readBodyToDto<Object<JobDto>>(controller->getDefaultObjectMapper());

            // Handle error if jobDto is null or invalid
            if (!jobDto) {
                return _return(controller->createResponse(Status::CODE_400, "Invalid Job Data"));
            }

            // Call async method on jobManager to create the job
            return jobManager->createJobAsync(jobDto->inputFile, jobDto->outputFile, jobDto->options)
                .callbackTo(&CreateJob::onJobCreated)
                .failTo(&CreateJob::onError);  // Error handling
        }

        Action onJobCreated(const oatpp::Int32& jobId) {
            auto responseDto = JobDto::createShared();
            responseDto->id = jobId;
            auto response = controller->createDtoResponse(Status::CODE_201, responseDto);
            return _return(response);
        }

        Action onError(const std::shared_ptr<const Error>& error) {
            return _return(controller->createResponse(Status::CODE_500, "Failed to create job: " + error->what()));
        }
    };

    // Asynchronous endpoint for retrieving a job by ID
    ENDPOINT_ASYNC("GET", "/jobs/{jobId}", GetJobById) {
        ENDPOINT_ASYNC_INIT(GetJobById)

        Action act() override {
            OATPP_COMPONENT(std::shared_ptr<JobManager>, jobManager);  // Inject JobManager component
            auto jobId = request->getPathVariable<oatpp::Int32>("jobId");

            // Call async method on jobManager to get the job
            return jobManager->getJobAsync(jobId)
                .callbackTo(&GetJobById::onJobRetrieved)
                .failTo(&GetJobById::onError);  // Error handling
        }

        Action onJobRetrieved(const std::shared_ptr<JobDto>& job) {
            if (job) {
                auto response = controller->createDtoResponse(Status::CODE_200, job);
                return _return(response);
            }
            return _return(controller->createResponse(Status::CODE_404, "Job not found"));
        }

        Action onError(const std::shared_ptr<const Error>& error) {
            return _return(controller->createResponse(Status::CODE_500, "Error retrieving job: " + error->what()));
        }
    };
};

#include OATPP_CODEGEN_END(ApiController) //<-- End codegen