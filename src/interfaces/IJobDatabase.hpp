#pragma once

#include "IDatabase.hpp"
#include <string>
#include <map>
#include <vector>

class IJobDatabase : public IDatabase {
public:
    virtual ~IJobDatabase() = default;

    // Job-specific CRUD Operations
    virtual int createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options, const std::string& remotePath) = 0;
    virtual bool updateJobStatus(int jobId, const std::string& status, const std::string& message = "") = 0;
    virtual std::map<std::string, std::string> getJob(int jobId) = 0; // Returns job details in a map
    virtual std::vector<std::map<std::string, std::string>> getAllJobs() = 0;
    virtual bool deleteJob(int jobId) = 0;
};