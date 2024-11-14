#include "SQLiteDatabase.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>

SQLiteDatabase::SQLiteDatabase(std::string dbPath)
    : m_dbPath(std::move(dbPath)), m_db(nullptr), m_connected(false) {}

SQLiteDatabase::~SQLiteDatabase() {
    std::lock_guard lock(m_mutex);
    if (m_connected && m_db) {
        sqlite3_close(m_db);
        m_connected = false;
        Logger::getInstance().info("Disconnected from SQLite database at " + m_dbPath);
    }
}

void SQLiteDatabase::connect() {
    std::lock_guard lock(m_mutex);
    if (m_connected) {
        Logger::getInstance().warn("Database is already connected.");
        return;
    }
    if (sqlite3_open(m_dbPath.c_str(), &m_db) == SQLITE_OK) {
        m_connected = true;
        Logger::getInstance().info("Connected to SQLite database at " + m_dbPath);
    } else {
        throw std::runtime_error("Failed to connect to SQLite database at " + m_dbPath);
    }
}

void SQLiteDatabase::disconnect() {
    std::lock_guard lock(m_mutex);
    if (m_connected && m_db) {
        sqlite3_close(m_db);
        m_connected = false;
        m_db = nullptr;
        Logger::getInstance().info("Disconnected from SQLite database at " + m_dbPath);
    }
}

bool SQLiteDatabase::isConnected() const {
    return m_connected;
}

int SQLiteDatabase::executeQuery(const std::string& query) {
    return executeQuery(query, {});
}

int SQLiteDatabase::executeQuery(const std::string& query, const std::vector<std::string>& params) {
    std::lock_guard lock(m_mutex);
    checkConnection();

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        Logger::getInstance().error("Failed to prepare query: " + query);
        return -1;
    }

    for (int i = 0; i < params.size(); ++i) {
        if (sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            Logger::getInstance().error("Failed to bind parameter " + std::to_string(i + 1));
            sqlite3_finalize(stmt);
            return -1;
        }
    }

    int affectedRows = (sqlite3_step(stmt) == SQLITE_DONE) ? sqlite3_changes(m_db) : -1;
    sqlite3_finalize(stmt);
    return affectedRows;
}

std::vector<std::vector<std::string>> SQLiteDatabase::fetchQuery(const std::string& query) {
    return fetchQuery(query, {});
}

std::vector<std::vector<std::string>> SQLiteDatabase::fetchQuery(const std::string& query, const std::vector<std::string>& params) {
    std::lock_guard lock(m_mutex);
    checkConnection();

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query: " + query);
    }

    for (int i = 0; i < params.size(); ++i) {
        if (sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Failed to bind parameter at index: " + std::to_string(i + 1));
        }
    }

    std::vector<std::vector<std::string>> results;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        int columnCount = sqlite3_column_count(stmt);
        for (int i = 0; i < columnCount; ++i) {
            const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.emplace_back(val ? val : "");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

int SQLiteDatabase::getLastInsertId() {
    std::lock_guard lock(m_mutex);
    checkConnection();
    return static_cast<int>(sqlite3_last_insert_rowid(m_db));
}

int SQLiteDatabase::executeInsertReturningId(const std::string& query, const std::vector<std::string>& params) {
    executeQuery(query, params);
    return getLastInsertId();
}

std::shared_ptr<ITransaction> SQLiteDatabase::beginTransaction() {
    executeQuery("BEGIN TRANSACTION;");
    return nullptr;
}

void SQLiteDatabase::checkConnection() const {
    if (!m_connected) {
        throw std::runtime_error("SQLiteDatabase is not connected.");
    }
}