#include "ConnectionPool.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>
#include <utility>

/**
 * @brief Constructs the ConnectionPool with a specified size and initializes connections.
 *
 * @param poolSize The number of connections to maintain in the pool.
 * @param connectionString The database connection string.
 * @throws std::runtime_error if initialization fails.
 */
ConnectionPool::ConnectionPool(const size_t poolSize, std::string  connectionString)
    : m_poolSize(poolSize), m_connectionString(std::move(connectionString)) {
    initializePool();
}

/**
 * @brief Initializes the pool with the specified number of connections.
 *
 * Attempts to establish `poolSize` connections. If any connection fails, the
 * pool initialization process will throw an error and log the failure.
 */
void ConnectionPool::initializePool() {
    for (size_t i = 0; i < m_poolSize; ++i) {
        if (auto conn = std::make_shared<DatabaseConnection>(m_connectionString); conn->isConnected()) {
            m_connections.push(conn);
        } else {
            Logger::getInstance().error("Failed to initialize a database connection.");
            cleanupPool(); // Ensure cleanup on failure
            throw std::runtime_error("Connection pool initialization failed.");
        }
    }
}

/**
 * @brief Acquires a connection from the pool, reconnecting if necessary.
 *
 * @return A shared pointer to an active DatabaseConnection.
 * @throws std::runtime_error if no connections are available and reconnection fails.
 */
std::shared_ptr<DatabaseConnection> ConnectionPool::acquire() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] { return !m_connections.empty(); });

    auto conn = m_connections.front();
    m_connections.pop();

    if (!conn->isConnected()) {
        Logger::getInstance().warn("Reconnecting a dropped database connection.");
        conn->connect();
        if (!conn->isConnected()) {
            throw std::runtime_error("Failed to reconnect database connection.");
        }
    }
    return conn;
}

/**
 * @brief Releases a connection back to the pool.
 *
 * Ensures the connection is connected before returning it to the pool. If not, it
 * attempts to reconnect.
 *
 * @param conn A shared pointer to the DatabaseConnection to release.
 */
void ConnectionPool::release(const std::shared_ptr<DatabaseConnection>& conn) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!conn->isConnected()) {
        Logger::getInstance().warn("Reconnecting before releasing a dropped database connection.");
        conn->connect();
    }

    m_connections.push(conn);
    m_condition.notify_one();
}

/**
 * @brief Cleans up all connections in the pool.
 *
 * Closes and removes each connection from the pool. Called in the destructor
 * and on initialization failure.
 */
void ConnectionPool::cleanupPool() {
    while (!m_connections.empty()) {
        const auto conn = m_connections.front();
        m_connections.pop();
        conn->disconnect();
    }
}

/**
 * @brief Destructor. Cleans up all remaining connections in the pool.
 */
ConnectionPool::~ConnectionPool() {
    cleanupPool();
}