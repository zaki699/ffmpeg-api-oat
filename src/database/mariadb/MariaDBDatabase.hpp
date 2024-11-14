#pragma once

#include "database/interfaces/IDatabase.hpp"
#include "database/interfaces/ITransaction.hpp"
#include <mariadb/mysql.h>
#include <string>
#include <vector>
#include <queue>
#include <condition_variable>

/**
 * @class MariaDBDatabase
 * @brief Manages connections and interactions with a MariaDB database, providing a thread-safe connection pool.
 *
 * This class supports connection management, query execution, and transaction handling for MariaDB,
 * leveraging a connection pool for efficient resource management in multithreaded applications.
 */
class MariaDBDatabase : public IDatabase {
public:
    /**
     * @brief Constructs a MariaDBDatabase instance with specified connection parameters and pool size.
     * @param host Database host address.
     * @param user Database username.
     * @param password Database password.
     * @param database Database name.
     * @param port Database port number.
     * @param poolSize Number of connections to maintain in the connection pool.
     */
    MariaDBDatabase(std::string host, std::string user, std::string password,
                    std::string database, unsigned int port, size_t poolSize);

    /**
     * @brief Destructor. Closes all connections in the pool.
     */
    ~MariaDBDatabase() override;

    /**
     * @brief Establishes the connection pool for the MariaDB database.
     * @throws std::runtime_error if the connection pool cannot be initialized.
     */
    void connect() override;

    /**
     * @brief Closes all active connections in the connection pool.
     */
    void disconnect() override;

    /**
     * @brief Checks if the database is connected by verifying the pool's status.
     * @return True if the pool has active connections, false otherwise.
     */
    [[nodiscard]] bool isConnected() const override;

    /**
     * @brief Executes a non-select SQL query on the database.
     *
     * @param query SQL query string to execute.
     * @return Number of affected rows if successful; -1 if an error occurs.
     */
    int executeQuery(const std::string& query) override;

    /**
     * @brief Executes a parameterized non-select SQL query on the database.
     *
     * @param query SQL query string with placeholders.
     * @param params A vector of values to bind to the query's placeholders, in sequential order.
     * @return Number of affected rows if successful; -1 if an error occurs.
     */
    int executeQuery(const std::string& query, const std::vector<std::string>& params) override;

    /**
     * @brief Executes a select query and fetches the results.
     *
     * @param query The SQL query string to execute.
     * @return A 2D vector containing the query result, where each row is a vector of column values.
     */
    std::vector<std::vector<std::string>> fetchQuery(const std::string& query) override;

    /**
     * @brief Executes a parameterized select query and fetches the results.
     *
     * @param query The SQL query string with placeholders.
     * @param params A vector of values to bind to the query's placeholders, in sequential order.
     * @return A 2D vector containing the query result, where each row is a vector of column values.
     */
    std::vector<std::vector<std::string>> fetchQuery(const std::string& query, const std::vector<std::string>& params) override;

    /**
     * @brief Begins a new transaction on the database.
     * @return A shared pointer to an ITransaction representing the transaction.
     * @throws std::runtime_error if the transaction cannot be started.
     */
    std::shared_ptr<ITransaction> beginTransaction() override;

private:
    std::string m_host;               ///< Database host address.
    std::string m_user;               ///< Database username.
    std::string m_password;           ///< Database password.
    std::string m_database;           ///< Database name.
    unsigned int m_port;              ///< Database port.
    size_t m_poolSize;                ///< Number of connections in the pool.
    bool m_connected;                 ///< Connection status flag.

    std::queue<MYSQL*> m_connectionPool; ///< Pool of MariaDB connections.
    std::mutex m_poolMutex;             ///< Mutex for thread-safe access to the connection pool.
    std::condition_variable m_condition; ///< Condition variable for managing connection availability.

    /**
     * @brief Initializes the pool with `m_poolSize` connections.
     * @throws std::runtime_error if any connection cannot be established.
     */
    void initializePool();

    /**
     * @brief Acquires a connection from the pool.
     * @return A pointer to an available MYSQL connection.
     */
    MYSQL* acquireConnection();

    /**
     * @brief Releases a connection back into the pool.
     * @param connection A pointer to the MYSQL connection to be returned.
     */
    void releaseConnection(MYSQL* connection);

    /**
     * @brief Ensures the database is connected before executing queries.
     * @throws std::runtime_error if the database is not connected.
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