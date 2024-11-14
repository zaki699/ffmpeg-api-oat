#include "DatabaseConnection.hpp"
#include "utils/Logger.hpp"

DatabaseConnection::DatabaseConnection(std::string  connectionString)
    : m_connectionString(std::move(connectionString)) {
    connect();
}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

void DatabaseConnection::connect() {
    if (!m_connected) {
        Logger::getInstance().info("Attempting to connect to the database.");
        // Connect to the database (implementation may vary by database type)
        Logger::getInstance().info("Connecting to database with connection string: " + m_connectionString);
        m_connected = true;
        Logger::getInstance().info("Successfully connected to the database.");
    } else {
        Logger::getInstance().warn("Connection attempt ignored: already connected.");
    }
}

void DatabaseConnection::disconnect() {
    if (m_connected) {
        Logger::getInstance().info("Disconnecting from the database.");
        // Close the connection (implementation may vary by database type)
        m_connected = false;
        Logger::getInstance().info("Successfully disconnected from the database.");
    } else {
        Logger::getInstance().warn("Disconnection attempt ignored: not currently connected.");
    }
}

bool DatabaseConnection::isConnected() const {
    return m_connected;
}