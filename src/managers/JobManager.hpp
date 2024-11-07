#pragma once

#include "IJobDatabase.hpp"
#include "IEncodingService.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

class JobManager {
public:
    JobManager(std::shared_ptr<IJobDatabase> jobDatabase, std::shared_ptr<IEncodingService> encodingService);

    int createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options);
    std::map<std::string, std::string> getJob(int jobId);
    bool updateJobStatus(int jobId, const std::string& status, const std::string& message = "");
    std::vector<std::map<std::string, std::string>> getAllJobs();
    bool deleteJob(int jobId);

private:
    std::shared_ptr<IJobDatabase> m_jobDatabase;
    std::shared_ptr<IEncodingService> m_encodingService;
};