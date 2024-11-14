#pragma once

#include <string>
#include "utils/Logger.hpp"

/**
 * @class DatabaseConnection
 * @brief Manages a database connection using a connection string.
 *
 * This class establishes a connection to a database upon instantiation,
 * and disconnects upon destruction, with connection state management.
 */
class DatabaseConnection {
public:
    /**
     * @brief Constructs a DatabaseConnection instance and connects to the database.
     * @param connectionString The connection string for the database.
     */
    explicit DatabaseConnection(std::string  connectionString);

    /**
     * @brief Destructor. Disconnects from the database if connected.
     */
    ~DatabaseConnection();

    /**
     * @brief Connects to the database if not already connected.
     */
    void connect();

    /**
     * @brief Disconnects from the database if connected.
     */
    void disconnect();

    /**
     * @brief Checks if the connection to the database is active.
     * @return True if connected, false otherwise.
     */
    [[nodiscard]] bool isConnected() const;

private:
    std::string m_connectionString; ///< Connection string used to connect to the database.
    bool m_connected = false;       ///< Indicates the connection status.
};