#include "JobRepository.hpp"

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include "dto/JobDto.hpp"
#include "database/QueryBuilder.hpp"

JobRepository::JobRepository(std::shared_ptr<IDatabase> database)
    : m_database(std::move(database)) {}

int JobRepository::createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options, const std::string& status) const {
        const std::string query = "INSERT INTO jobs (inputFile, outputFile, options, status) VALUES (?, ?, ?, ?);";
        return m_database->executeInsertReturningId(query, {inputFile, outputFile, options, status});
}

std::shared_ptr<JobDto> JobRepository::getJobById(const int jobId) const {
        QueryBuilder builder;
        builder.select({"id", "inputFile", "outputFile", "options", "status"})
               .from("jobs")
               .where("id = " + std::to_string(jobId));

        if (const auto result = m_database->fetchQuery(builder.build()); !result.empty()) {
            return mapToJobDto(result[0]);
        }
        return nullptr;
}

std::vector<std::shared_ptr<JobDto>> JobRepository::getAllJobs(const std::string& status ) const {
        QueryBuilder builder;
        builder.select({"id", "inputFile", "outputFile", "options", "status"})
               .from("jobs");

        if (!status.empty()) {
            builder.where("status = '" + status + "'");
        }

        const auto results = m_database->fetchQuery(builder.build());
        std::vector<std::shared_ptr<JobDto>> jobs;
        jobs.reserve(results.size());
        for (const auto& row : results) {
            jobs.push_back(mapToJobDto(row));
        }
        return jobs;
}

bool JobRepository::updateJobStatus(const int jobId, const std::string& status, const std::string& message) const {
        const std::string query = "UPDATE jobs SET status = ?, message = ? WHERE id = ?;";
        return m_database->executeQuery(query, {status, message, std::to_string(jobId)});
}

bool JobRepository::deleteJob(const int jobId) const {
        const std::string query = "DELETE FROM jobs WHERE id = ?;";
        return m_database->executeQuery(query, {std::to_string(jobId)});
}

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
std::shared_ptr<JobDto> JobRepository::mapToJobDto(const std::vector<std::string>& row) {
        if (row.size() < 5) {
            throw std::runtime_error("Invalid row format for JobDto mapping");
        }

        const auto jobDto = JobDto::createShared();
        jobDto->id = std::stoi(row[0]);
        jobDto->inputFile = row[1];
        jobDto->outputFile = row[2];
        jobDto->options = row[3];

        // Convert the status from std::string to JobStatus enum
        try {
            jobDto->status = JobStatusUtils::fromString(row[4]);  // Assuming JobStatus::fromString exists
        } catch (...) {
            Logger::getInstance().warn("Invalid job status : " + row[4]);
            jobDto->status = JobStatus::UNKNOWN; // Assign a default value if the status is unrecognized
        }

        return jobDto.getPtr(); // Use getPtr() to return std::shared_ptr<JobDto>
}
