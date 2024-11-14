#pragma once

#include <memory>
#include <vector>
#include <string>
#include "database/interfaces/IDatabase.hpp"
#include "dto/JobDto.hpp"

/**
 * @class JobRepository
 * @brief Manages CRUD operations for job records in the database.
 */
class JobRepository {
public:
    /**
     * @brief Constructs a JobRepository with the specified database.
     * @param database Shared pointer to the database interface.
     */
    explicit JobRepository(std::shared_ptr<IDatabase> database);

    /**
     * @brief Creates a new job entry in the database.
     * @param inputFile Input file path for the job.
     * @param outputFile Output file path for the job.
     * @param options Additional options for the job.
     * @param status Initial status of the job.
     * @return The ID of the newly created job.
     * @throws std::runtime_error if job creation fails.
     */
    [[nodiscard]] int createJob(const std::string& inputFile, const std::string& outputFile,
                                const std::string& options, const std::string& status) const;

    /**
     * @brief Retrieves a job by its ID.
     * @param jobId ID of the job to retrieve.
     * @return Shared pointer to the JobDto if found, otherwise nullptr.
     */
    [[nodiscard]] std::shared_ptr<JobDto> getJobById(int jobId) const;

    /**
     * @brief Retrieves all jobs, optionally filtering by status.
     * @param status Optional filter for job status.
     * @return Vector of shared pointers to JobDto objects.
     */
    [[nodiscard]] std::vector<std::shared_ptr<JobDto>> getAllJobs(const std::string& status = "") const;

    /**
     * @brief Updates the status and message of a job by ID.
     * @param jobId ID of the job to update.
     * @param status New status for the job.
     * @param message Optional message to associate with the job.
     * @return True if the job was successfully updated; otherwise, false.
     */
    [[nodiscard]] bool updateJobStatus(int jobId, const std::string& status, const std::string& message = "") const;

    /**
     * @brief Deletes a job by ID.
     * @param jobId ID of the job to delete.
     * @return True if the job was successfully deleted; otherwise, false.
     */
    [[nodiscard]] bool deleteJob(int jobId) const;

private:
    std::shared_ptr<IDatabase> m_database;

    /**
     * @brief Maps a database row to a JobDto.
     *
     * This function takes a vector of strings representing a database row and maps it to
     * a JobDto object. It converts the ID, inputFile, outputFile, options, and status fields.
     *
     * @param row A vector of strings, with each element representing a column value.
     * @return A shared pointer to a JobDto with fields populated from the row.
     * @throws std::runtime_error if the row format is invalid.
     */
    [[nodiscard]] static std::shared_ptr<JobDto> mapToJobDto(const std::vector<std::string>& row);
};