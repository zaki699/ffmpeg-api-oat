#include "SQLiteDatabase.hpp"
#include "Logger.hpp"
#include <stdexcept>

SQLiteDatabase::SQLiteDatabase(const std::string& dbPath) : db(nullptr) {
    connect(dbPath);
}

SQLiteDatabase::~SQLiteDatabase() {
    disconnect();
}

bool SQLiteDatabase::connect(const std::string& connectionString) {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (sqlite3_open(connectionString.c_str(), &db) == SQLITE_OK) {
        Logger::getInstance().info("Connected to SQLite database successfully.");
        return true;
    } else {
        logSQLError("connect", sqlite3_errcode(db));
        return false;
    }
}

bool SQLiteDatabase::disconnect() {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        Logger::getInstance().info("Disconnected from SQLite database.");
        return true;
    }
    return false;
}

bool SQLiteDatabase::isConnected() const {
    return db != nullptr;
}

int SQLiteDatabase::createJob(const std::string& inputFile, const std::string& outputFile, const std::string& options, const std::string& remotePath) {
    std::lock_guard<std::mutex> lock(dbMutex);
    const std::string query = "INSERT INTO jobs (input_file, output_file, options, remote_path, status) VALUES (?, ?, ?, ?, 'pending');";
    sqlite3_stmt* stmt;

    if (!prepareStatement(&stmt, query)) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, inputFile.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, outputFile.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, options.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, remotePath.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        logSQLError("createJob", result);
        finalizeStatement(stmt);
        return -1;
    }

    int jobId = static_cast<int>(sqlite3_last_insert_rowid(db));
    finalizeStatement(stmt);
    Logger::getInstance().info("Job created with ID: {}", jobId);
    return jobId;
}

bool SQLiteDatabase::updateJobStatus(int jobId, const std::string& status, const std::string& message) {
    std::lock_guard<std::mutex> lock(dbMutex);
    const std::string query = "UPDATE jobs SET status = ?, message = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (!prepareStatement(&stmt, query)) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, message.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, jobId);

    int result = sqlite3_step(stmt);
    bool success = (result == SQLITE_DONE);
    if (!success) {
        logSQLError("updateJobStatus", result);
    }
    finalizeStatement(stmt);
    return success;
}

std::shared_ptr<Job> SQLiteDatabase::getJob(int jobId) {
    std::lock_guard<std::mutex> lock(dbMutex);
    const std::string query = "SELECT * FROM jobs WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (!prepareStatement(&stmt, query)) {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, jobId);

    std::shared_ptr<Job> job = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        job = std::make_shared<Job>();
        job->id = sqlite3_column_int(stmt, 0);
        job->inputFile = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        job->outputFile = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        job->options = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        job->remotePath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        job->status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        job->message = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    }

    finalizeStatement(stmt);
    return job;
}

std::vector<std::shared_ptr<Job>> SQLiteDatabase::getAllJobs() {
    std::lock_guard<std::mutex> lock(dbMutex);
    const std::string query = "SELECT * FROM jobs;";
    sqlite3_stmt* stmt;

    std::vector<std::shared_ptr<Job>> jobs;
    if (!prepareStatement(&stmt, query)) {
        return jobs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto job = std::make_shared<Job>();
        job->id = sqlite3_column_int(stmt, 0);
        job->inputFile = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        job->outputFile = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        job->options = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        job->remotePath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        job->status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        job->message = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        jobs.push_back(job);
    }

    finalizeStatement(stmt);
    return jobs;
}

bool SQLiteDatabase::deleteJob(int jobId) {
    std::lock_guard<std::mutex> lock(dbMutex);
    const std::string query = "DELETE FROM jobs WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (!prepareStatement(&stmt, query)) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, jobId);

    int result = sqlite3_step(stmt);
    bool success = (result == SQLITE_DONE);
    if (!success) {
        logSQLError("deleteJob", result);
    }

    finalizeStatement(stmt);
    return success;
}

bool SQLiteDatabase::prepareStatement(sqlite3_stmt** stmt, const std::string& query) {
    if (sqlite3_prepare_v2(db, query.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
        logSQLError("prepareStatement", sqlite3_errcode(db));
        return false;
    }
    return true;
}

void SQLiteDatabase::finalizeStatement(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

void SQLiteDatabase::logSQLError(const std::string& action, int errorCode) {
    const char* errorMessage = sqlite3_errmsg(db);
    Logger::getInstance().error("SQLite error during {}: [{}] {}", action, errorCode, errorMessage);
}