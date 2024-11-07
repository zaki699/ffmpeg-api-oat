#ifndef JOB_HPP
#define JOB_HPP

#include <string>
#include <chrono>
#include "Logger.hpp"

enum class JobStatus {
    PENDING,
    IN_PROGRESS,
    COMPLETED,
    FAILED,
    CANCELLED
};

class Job {
public:
    Job(const std::string& id, const std::string& inputFile, const std::string& outputFile)
        : id(id), inputFile(inputFile), outputFile(outputFile), status(JobStatus::PENDING), attemptCount(0) {}

    // Getter functions
    std::string getId() const { return id; }
    std::string getInputFile() const { return inputFile; }
    std::string getOutputFile() const { return outputFile; }
    JobStatus getStatus() const { return status; }
    int getAttemptCount() const { return attemptCount; }

    // Setter functions
    void setStatus(JobStatus newStatus) { status = newStatus; }
    void incrementAttemptCount() { ++attemptCount; }

    // Logging function to log job details
    void logJobDetails() const {
        Logger::getInstance().info("Job ID: " + id + " | Input: " + inputFile + " | Output: " + outputFile);
    }

private:
    std::string id;
    std::string inputFile;
    std::string outputFile;
    JobStatus status;
    int attemptCount;
};

#endif // JOB_HPP