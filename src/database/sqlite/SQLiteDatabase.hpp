#pragma once

#include "database/interfaces/IDatabase.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>


/**
 * @class SQLiteDatabase
 * @brief Manages connections and interactions with an SQLite database.
 */
class SQLiteDatabase final : public IDatabase {
public:
    /**
     * @brief Constructs an SQLiteDatabase with the given database path.
     * @param dbPath Path to the SQLite database file.
     */
    explicit SQLiteDatabase(std::string dbPath);

    /**
     * @brief Destructor that closes the database connection if open.
     */
    ~SQLiteDatabase() override;

    void connect() override;
    void disconnect() override;
    [[nodiscard]] bool isConnected() const override;

    int executeQuery(const std::string& query) override;
    int executeQuery(const std::string& query, const std::vector<std::string>& params) override;

    std::vector<std::vector<std::string>> fetchQuery(const std::string& query) override;
    std::vector<std::vector<std::string>> fetchQuery(const std::string& query, const std::vector<std::string>& params) override;

    int getLastInsertId() override;
    int executeInsertReturningId(const std::string& query, const std::vector<std::string>& params) override;

    std::shared_ptr<ITransaction> beginTransaction() override;

private:
    void checkConnection() const;

    std::string m_dbPath;
    sqlite3* m_db;
    bool m_connected;
    mutable std::mutex m_mutex;
};