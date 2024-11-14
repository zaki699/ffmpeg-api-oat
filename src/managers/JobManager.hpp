#pragma once

#include <memory>
#include <vector>
#include "dto/JobDto.hpp"
#include "encoding/JobProcessor.hpp"
#include "interfaces/IEncodingService.hpp"
#include "repositories//JobRepository.hpp"
#include "oatpp/core/Types.hpp"

/**
 * @class JobManager
 * @brief Manages job creation, retrieval, and updates, coordinating between JobRepository, JobProcessor, and encoding service.
 */
class JobManager {
public:
    /**
     * @brief Constructs a JobManager with a job repository, encoding service, and job processor.
     * @param jobRepository A shared pointer to the JobRepository for accessing job storage.
     * @param encodingService A shared pointer to the IEncodingService interface for encoding operations.
     * @param jobProcessor A shared pointer to the JobProcessor for handling background job processing.
     */
    JobManager(const std::shared_ptr<JobRepository>& jobRepository,
               const std::shared_ptr<IEncodingService>& encodingService,
               const std::shared_ptr<JobProcessor>& jobProcessor);

    /**
     * @brief Creates a new job, stores it in the repository, and queues it for processing.
     * @param inputFile The input file path for the job.
     * @param outputFile The output file path for the job.
     * @param options Additional options for job processing.
     * @return The ID assigned to the created job.
     * @throws std::runtime_error if the job cannot be created.
     */
    [[nodiscard]] oatpp::Int32 createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options) const;

    /**
     * @brief Retrieves a specific job by ID.
     * @param jobId The ID of the job to retrieve.
     * @return A shared pointer to a JobDto representing the job.
     * @throws std::runtime_error if the job is not found.
     */
    [[nodiscard]] std::shared_ptr<JobDto> getJob(int jobId) const;

    /**
     * @brief Retrieves all jobs from the repository.
     * @return A vector of shared pointers to JobDto objects representing each job.
     */
    [[nodiscard]] std::vector<std::shared_ptr<JobDto>> getAllJobs() const;

    /**
     * @brief Updates the status and message for a specified job.
     * @param jobId The ID of the job to update.
     * @param status The new status to set for the job.
     * @param message An optional message describing the status update.
     * @return true if the update was successful; otherwise, false.
     */
    [[nodiscard]] bool updateJobStatus(int jobId, JobStatus status, const std::string& message) const;

    /**
     * @brief Deletes a job by its unique ID.
     * @param jobId The ID of the job to delete.
     * @return true if the job was successfully deleted; otherwise, false.
     */
    [[nodiscard]] bool deleteJob(int jobId) const;

private:
    std::shared_ptr<JobRepository> m_jobRepository;           ///< Job repository for managing job data.
    std::shared_ptr<IEncodingService> m_encodingService;      ///< Encoding service for processing jobs.
    std::shared_ptr<JobProcessor> m_jobProcessor;             ///< Processor for managing background job execution.
};