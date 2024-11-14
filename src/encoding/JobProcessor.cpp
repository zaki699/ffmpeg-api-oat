#include "JobProcessor.hpp"
#include "utils/Logger.hpp"

JobProcessor::JobProcessor(std::shared_ptr<IEncodingService> encodingService, std::shared_ptr<JobRepository> jobRepository)
    : m_encodingService(std::move(encodingService)),
      m_jobRepository(std::move(jobRepository)),
      m_running(false) {}

JobProcessor::~JobProcessor() {
    stop();
}

void JobProcessor::addJob(const std::shared_ptr<Job>& job) {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_jobQueue.push(job);
        Logger::getInstance().info("Job added to queue with ID: " + std::to_string(job->getId()));
    }
    m_condition.notify_one();  // Notify the processing thread
}

void JobProcessor::start() {
    if (m_running.load()) {
        Logger::getInstance().warn("JobProcessor is already running.");
        return;
    }

    m_running.store(true);
    m_processingThread = std::thread(&JobProcessor::processJobs, this);
    Logger::getInstance().info("JobProcessor started.");
}

void JobProcessor::stop() {
    if (!m_running.load()) {
        Logger::getInstance().warn("JobProcessor is not running.");
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_running.store(false);
    }
    m_condition.notify_all();  // Wake up the processing thread to exit

    if (m_processingThread.joinable()) {
        m_processingThread.join();
    }
    Logger::getInstance().info("JobProcessor stopped.");
}

void JobProcessor::processJobs() {
    while (m_running.load()) {
        std::unique_lock<std::mutex> lock(m_queueMutex);

        // Wait until there is a job in the queue or until stop() is called
        m_condition.wait(lock, [this] { return !m_jobQueue.empty() || !m_running.load(); });

        // Exit if stop() was called
        if (!m_running.load()) break;

        if (!m_jobQueue.empty()) {
            auto job = m_jobQueue.front();
            m_jobQueue.pop();
            lock.unlock();  // Unlock the queue while processing the job

            processJob(job);
        }
    }
}

void JobProcessor::processJob(const std::shared_ptr<Job>& job) const {
    // Update job status to "IN_PROGRESS" in the repository
    bool success = m_jobRepository->updateJobStatus(job->getId(), JobStatusUtils::toString(JobStatus::IN_PROGRESS));
    Logger::getInstance().info("Processing job ID: " + std::to_string(job->getId()));

    // Perform the encoding task using the encoding service
    success = m_encodingService->encode(job->getInputFile(), job->getOutputFile(), job->getOptions());

    // Update job status based on the encoding result
    const JobStatus newStatus = success ? JobStatus::COMPLETED : JobStatus::FAILED;
    success = m_jobRepository->updateJobStatus(job->getId(), JobStatusUtils::toString(newStatus));

    // Log the outcome
    if (success) {
        Logger::getInstance().info("Job completed successfully: ID " + std::to_string(job->getId()));
    } else {
        Logger::getInstance().error("Job failed: ID " + std::to_string(job->getId()));
    }
}
