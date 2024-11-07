#pragma once

#include "IJobDatabase.hpp"
#include "Job.hpp"
#include <sqlite3.h>
#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include "Logger.hpp"

class SQLiteDatabase : public IJobDatabase {
public:
    explicit SQLiteDatabase(const std::string& dbPath);
    ~SQLiteDatabase();

    bool connect(const std::string& connectionString) override;
    bool disconnect() override;
    bool isConnected() const override;

    int createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options, const std::string& remotePath) override;
    bool updateJobStatus(int jobId, const std::string& status, const std::string& message = "") override;
    std::shared_ptr<Job> getJob(int jobId) override;
    std::vector<std::shared_ptr<Job>> getAllJobs() override;
    bool deleteJob(int jobId) override;

private:
    sqlite3* db;
    std::mutex dbMutex;

    bool executeQuery(const std::string& query);
    bool prepareStatement(sqlite3_stmt** stmt, const std::string& query);
    void finalizeStatement(sqlite3_stmt* stmt);
    void logSQLError(const std::string& action, int errorCode);
};