#ifndef JOB_HPP
#define JOB_HPP

#include <string>
#include <vector>
#include "utils/Logger.hpp"
#include "JobStatus.hpp"

// Job class representing a job in the system
class Job {
public:
    // Constructor
    Job(int id, std::string inputFile, std::string outputFile, std::vector<std::string> options = {}, std::string remotePath = "")
        : id(id), inputFile(std::move(inputFile)), outputFile(std::move(outputFile)), options(std::move(options)),
          remotePath(std::move(remotePath)), status(JobStatus::PENDING), attemptCount(0) {}

    // Getter functions
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] std::string getInputFile() const { return inputFile; }
    [[nodiscard]] std::string getOutputFile() const { return outputFile; }
    [[nodiscard]] const std::vector<std::string>& getOptions() const { return options; }
    [[nodiscard]] std::string getRemotePath() const { return remotePath; }
    [[nodiscard]] JobStatus getStatus() const { return status; }
    [[nodiscard]] std::string getStatusString() const { return JobStatusUtils::toString(status); }
    [[nodiscard]] int getAttemptCount() const { return attemptCount; }
    [[nodiscard]] std::string getMessage() const { return message; }

    // Setter functions
    void setStatus(JobStatus newStatus) { status = newStatus; }
    void setStatus(const std::string& statusStr) { status = JobStatusUtils::fromString(statusStr); }
    void setMessage(const std::string& msg) { message = msg; }
    void setOptions(const std::vector<std::string>& opts) { options = opts; }
    void setRemotePath(const std::string& path) { remotePath = path; }
    void incrementAttemptCount() { ++attemptCount; }

    // Logging function to log job details
    void logJobDetails() const {
        Logger::getInstance().info("Job ID: " + std::to_string(id) + " | Input: " + inputFile + " | Output: " + outputFile +
                           " | Status: " + getStatusString() + " | Attempts: " + std::to_string(attemptCount));
    }

private:
    int id;
    std::string inputFile;
    std::string outputFile;
    std::vector<std::string> options; // Changed to std::vector<std::string>
    std::string remotePath;
    JobStatus status;
    int attemptCount;
    std::string message;  // Error or status message for the job
};

#endif // JOB_HPP