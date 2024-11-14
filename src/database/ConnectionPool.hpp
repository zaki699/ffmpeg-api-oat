#pragma once

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "DatabaseConnection.hpp"

/**
 * @class ConnectionPool
 * @brief Manages a pool of database connections, providing acquisition and release mechanisms.
 */
class ConnectionPool {
public:
 /**
  * @brief Constructs the ConnectionPool with a specified size and initializes connections.
  *
  * @param poolSize The number of connections to maintain in the pool.
  * @param connectionString The database connection string.
  */
 ConnectionPool(size_t poolSize, std::string  connectionString);

 /**
  * @brief Acquires a connection from the pool, reconnecting if necessary.
  *
  * @return A shared pointer to an active DatabaseConnection.
  */
 std::shared_ptr<DatabaseConnection> acquire();

 /**
  * @brief Releases a connection back to the pool.
  *
  * @param conn A shared pointer to the DatabaseConnection to release.
  */
 void release(const std::shared_ptr<DatabaseConnection>& conn);

 /**
  * @brief Destructor. Cleans up all remaining connections in the pool.
  */
 ~ConnectionPool();

private:
 size_t m_poolSize; ///< Number of connections in the pool.
 std::string m_connectionString; ///< Database connection string.
 std::queue<std::shared_ptr<DatabaseConnection>> m_connections; ///< Pool of connections.
 std::mutex m_mutex; ///< Mutex to synchronize access to the pool.
 std::condition_variable m_condition; ///< Condition variable for connection availability.

 /**
  * @brief Initializes the pool with the specified number of connections.
  */
 void initializePool();

 /**
  * @brief Cleans up all connections in the pool, closing each one.
  */
 void cleanupPool();
};