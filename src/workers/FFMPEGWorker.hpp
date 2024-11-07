#ifndef FFMPEGWORKER_HPP
#define FFMPEGWORKER_HPP

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <fstream>
#include <json.hpp>  // For parsing configuration files
#include "Job.hpp"
#include "Logger.hpp"

using json = nlohmann::json;

class FFMPEGWorker {
public:
    FFMPEGWorker(const std::string& configFilePath);

    void enqueueJob(const std::shared_ptr<Job>& job);
    void startProcessing();
    void stopProcessing();

private:
    int maxProcesses;
    int maxRetries;
    int retryDelaySeconds;
    json config;
    std::queue<std::shared_ptr<Job>> jobQueue;
    std::vector<std::thread> workerThreads;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    bool stopRequested = false;

    void loadConfig(const std::string& configFilePath);
    void processJobs();
    void processJob(const std::shared_ptr<Job>& job);
    std::string buildFFmpegCommand(const std::shared_ptr<Job>& job);
    int executeFFmpegCommand(const std::string& command, std::string& output);
};

#endif // FFMPEGWORKER_HPP