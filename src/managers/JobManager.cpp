#include "JobManager.hpp"
#include "utils/Logger.hpp"

JobManager::JobManager(const std::shared_ptr<JobRepository>& jobRepository,
                       const std::shared_ptr<IEncodingService>& encodingService,
                       const std::shared_ptr<JobProcessor>& jobProcessor)
    : m_jobRepository(jobRepository),
      m_encodingService(encodingService),
      m_jobProcessor(jobProcessor) {}

oatpp::Int32 JobManager::createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options) const {
    if (int jobId = m_jobRepository->createJob(inputFile, outputFile, options, "PENDING"); jobId != -1) {
        Logger::getInstance().info("Job created with ID: " + std::to_string(jobId));

        // Convert options to a vector and create the Job instance
        m_jobProcessor->addJob(std::make_shared<Job>(jobId, inputFile, outputFile, std::vector<std::string>{options}));

        return jobId;
    } else {
        Logger::getInstance().error("Failed to create job");
        throw std::runtime_error("Failed to create job");
    }
}

std::shared_ptr<JobDto> JobManager::getJob(int jobId) const {
    if (const auto job = m_jobRepository->getJobById(jobId)) {
        auto jobDto = JobDto::createShared();
        jobDto->id = job->id;
        jobDto->inputFile = job->inputFile;
        jobDto->outputFile = job->outputFile;
        jobDto->status = job->status;
        return jobDto.getPtr();
    } else {
        throw std::runtime_error("Job not found");
    }
}

std::vector<std::shared_ptr<JobDto>> JobManager::getAllJobs() const {
    const auto jobs = m_jobRepository->getAllJobs();
    std::vector<std::shared_ptr<JobDto>> jobDtos;
    jobDtos.reserve(jobs.size());

    for (const auto& job : jobs) {
        auto jobDto = JobDto::createShared();
        jobDto->id = job->id;
        jobDto->inputFile = job->inputFile;
        jobDto->outputFile = job->outputFile;
        jobDto->status = job->status;
        jobDtos.push_back(jobDto.getPtr());
    }
    return jobDtos;
}

bool JobManager::updateJobStatus(const int jobId, const JobStatus status, const std::string& message) const {
    return m_jobRepository->updateJobStatus(jobId, JobStatusUtils::toString(status), message);
}

bool JobManager::deleteJob(const int jobId) const {
    return m_jobRepository->deleteJob(jobId);
}