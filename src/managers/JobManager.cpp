#include "JobManager.hpp"
#include "Logger.hpp"

JobManager::JobManager(std::shared_ptr<IJobDatabase> jobDatabase, std::shared_ptr<IEncodingService> encodingService)
    : m_jobDatabase(std::move(jobDatabase)), m_encodingService(std::move(encodingService)) {}

int JobManager::createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options) {
    int jobId = m_jobDatabase->createJob(inputFile, outputFile, options, "");
    if (jobId != -1) {
        Logger::getInstance().info("Job created with ID: " + std::to_string(jobId));
    } else {
        Logger::getInstance().error("Failed to create job");
    }
    return jobId;
}

std::map<std::string, std::string> JobManager::getJob(int jobId) {
    return m_jobDatabase->getJob(jobId);
}

bool JobManager::updateJobStatus(int jobId, const std::string& status, const std::string& message) {
    return m_jobDatabase->updateJobStatus(jobId, status, message);
}

std::vector<std::map<std::string, std::string>> JobManager::getAllJobs() {
    return m_jobDatabase->getAllJobs();
}

bool JobManager::deleteJob(int jobId) {
    return m_jobDatabase->deleteJob(jobId);
}