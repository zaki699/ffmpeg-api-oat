#pragma once

#include <queue>
#include <thread>
#include <condition_variable>
#include "interfaces/IEncodingService.hpp"
#include "models/Job.hpp"
#include "repositories/JobRepository.hpp"

/**
 * @class JobProcessor
 * @brief Manages the processing of encoding jobs in a background thread.
 *
 * The JobProcessor class maintains a queue of jobs to be processed and uses an encoding service
 * to perform encoding tasks. Job states are updated in the JobRepository.
 */
class JobProcessor {
public:
    /**
     * @brief Constructs a JobProcessor with the specified encoding service and job repository.
     *
     * @param encodingService A shared pointer to an encoding service used to process jobs.
     * @param jobRepository A shared pointer to the job repository used to manage job states.
     */
    JobProcessor(std::shared_ptr<IEncodingService> encodingService, std::shared_ptr<JobRepository> jobRepository);

    /**
     * @brief Destructor that stops the job processing thread if it's running.
     */
    ~JobProcessor();

    /**
     * @brief Adds a new job to the processing queue.
     *
     * @param job A shared pointer to the job to add to the queue.
     */
    void addJob(const std::shared_ptr<Job>& job);

    /**
     * @brief Starts the job processing in a background thread.
     */
    void start();

    /**
     * @brief Stops the job processing, blocking until the background thread exits.
     */
    void stop();

private:
    /**
     * @brief Continuously processes jobs from the queue until stopped.
     */
    void processJobs();

    /**
     * @brief Processes a single job, updating its status based on encoding success or failure.
     *
     * @param job A shared pointer to the job to process.
     */
    void processJob(const std::shared_ptr<Job>& job) const;

    std::shared_ptr<IEncodingService> m_encodingService;  ///< Encoding service for processing jobs.
    std::shared_ptr<JobRepository> m_jobRepository;       ///< Job repository for managing job states.
    std::queue<std::shared_ptr<Job>> m_jobQueue;          ///< Queue of jobs to be processed.
    std::mutex m_queueMutex;                              ///< Mutex for synchronizing access to the job queue.
    std::condition_variable m_condition;                  ///< Condition variable for signaling the processing thread.
    std::thread m_processingThread;                       ///< Thread for processing jobs in the background.
    std::atomic<bool> m_running;                          ///< Flag to control the processing thread.
};