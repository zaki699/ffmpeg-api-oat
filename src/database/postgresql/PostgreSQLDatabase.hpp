#pragma once

#include "database/interfaces/IDatabase.hpp"
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <queue>
#include <condition_variable>

/**
 * @class PostgreSQLDatabase
 * @brief Manages connections and interactions with a PostgreSQL database, using a connection pool for multithreaded efficiency.
 */
class PostgreSQLDatabase final : public IDatabase {
public:
    /**
     * @brief Constructs a PostgreSQLDatabase instance with specified connection string and pool size.
     * @param conninfo Connection string for the PostgreSQL database.
     * @param poolSize Number of connections to maintain in the connection pool.
     */
    PostgreSQLDatabase(std::string conninfo, size_t poolSize);

    /**
     * @brief Destructor. Ensures all connections are closed.
     */
    ~PostgreSQLDatabase() override;

    /**
     * @brief Establishes a connection pool for the PostgreSQL database.
     * @throws std::runtime_error if the connection pool cannot be initialized.
     */
    void connect() override;

    /**
     * @brief Closes all connections in the connection pool.
     */
    void disconnect() override;

    /**
     * @brief Checks if there is an active connection pool.
     * @return True if the pool has active connections, false otherwise.
     */
    [[nodiscard]] bool isConnected() const override;

    /**
     * @brief Executes a non-select SQL query on the database.
     * @param query SQL query string to execute.
     * @return Number of affected rows if successful; -1 if an error occurs.
     */
    int executeQuery(const std::string& query) override;

    /**
     * @brief Executes a parameterized non-select SQL query on the database.
     * @param query SQL query string with placeholders.
     * @param params Vector of parameter values for the query.
     * @return Number of affected rows if successful; -1 if an error occurs.
     */
    int executeQuery(const std::string& query, const std::vector<std::string>& params) override;

    /**
     * @brief Executes a select SQL query and retrieves the result set.
     * @param query SQL query string to execute.
     * @return A 2D vector containing the query result, where each row is a vector of strings.
     * @throws std::runtime_error if the query fails.
     */
    std::vector<std::vector<std::string>> fetchQuery(const std::string& query) override;

    /**
     * @brief Executes a parameterized select SQL query and retrieves the result set.
     * @param query SQL query string with placeholders.
     * @param params Vector of parameter values for the query.
     * @return A 2D vector containing the query result, where each row is a vector of strings.
     * @throws std::runtime_error if the query fails.
     */
    std::vector<std::vector<std::string>> fetchQuery(const std::string& query, const std::vector<std::string>& params) override;

    /**
     * @brief Begins a new database transaction.
     * @return A shared pointer to an ITransaction representing the transaction.
     * @throws std::runtime_error if the transaction cannot be started.
     */
    std::shared_ptr<ITransaction> beginTransaction() override;

private:
    std::string m_conninfo;            ///< Connection string for PostgreSQL database.
    size_t m_poolSize;                 ///< Number of connections in the pool.
    std::queue<PGconn*> m_connectionPool; ///< Pool of PostgreSQL connections.
    mutable std::mutex m_poolMutex;    ///< Mutex for thread-safe connection pool access.
    std::condition_variable m_condition; ///< Condition variable for managing connection availability.
    bool m_connected;                  ///< Connection status flag for the pool.

    /**
     * @brief Initializes the connection pool with `m_poolSize` connections.
     * @throws std::runtime_error if a connection cannot be established.
     */
    void initializePool();

    /**
     * @brief Retrieves a connection from the pool, waiting if necessary.
     * @return A pointer to a `PGconn` from the pool.
     */
    PGconn* acquireConnection();

    /**
     * @brief Releases a connection back into the pool.
     * @param conn A pointer to a `PGconn` to return to the pool.
     */
    void releaseConnection(PGconn* conn);

    /**
     * @brief Checks if the database is connected and throws an exception if it is not.
     * @throws std::runtime_error if the connection pool is not initialized.
     */
    void checkConnection() const;

    /**
     * @brief Cleans up all connections in the connection pool, closing each one.
     */
    void cleanupPool();

   /**
    * @brief Retrieves the last inserted row ID for the database.
    *
    * @return The last inserted row ID as an integer.
    */
    int getLastInsertId() override;

   /**
    * @brief Executes an insert query with parameters and retrieves the last inserted ID.
    * @param query SQL query string with placeholders.
    * @param params A vector of values to bind to the query's placeholders, in sequential order.
    * @return The last inserted row ID, or -1 if not applicable or an error occurs.
    */
   int executeInsertReturningId(const std::string& query, const std::vector<std::string>& params) override;

};