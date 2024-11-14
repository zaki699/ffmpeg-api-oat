#pragma once

#include "interfaces/IEncodingService.hpp"
#include "database/interfaces/IDatabase.hpp"
#include "utils/ConfigManager.hpp"

/**
 * @brief PluginManager manages the initialization and retrieval of various services like encoding and database.
 */
class PluginManager {
public:
 PluginManager() = default;  // Private constructor for singleton
 ~PluginManager() = default;
    /**
     * @brief Retrieves the singleton instance of PluginManager.
     *
     * @return PluginManager& - Reference to the singleton instance.
     */
    static PluginManager& getInstance();

    /**
     * @brief Initializes all plugins based on configuration.
     *
     * Loads each configured service (encoding, database) and establishes connections if needed.
     * @throws std::runtime_error if any plugin fails to initialize.
     */
    void initialize();

    /**
     * @brief Disconnects and shuts down all plugins.
     */
    void shutdown() const;

    /**
     * @brief Retrieves the configured encoding service.
     *
     * @return std::shared_ptr<IEncodingService> - Shared pointer to the encoding service.
     */
    [[nodiscard]] std::shared_ptr<IEncodingService> getEncodingService() const;

    /**
     * @brief Retrieves the configured database instance.
     *
     * @return std::shared_ptr<IDatabase> - Shared pointer to the database.
     */
    [[nodiscard]] std::shared_ptr<IDatabase> getDatabase() const;

private:



    /**
     * @brief Loads and initializes the encoding service based on the configuration.
     *
     * Currently supports "ffmpeg" as an example. Throws an exception if an unknown service is provided.
     *
     * @param serviceName The name of the encoding service to load.
     * @throws std::runtime_error if the encoding service is unknown.
     */
    void loadEncodingService(const std::string& serviceName);

    /**
     * @brief Loads and initializes the database based on the configuration.
     *
     * Supports SQLite, MariaDB, and PostgreSQL. Throws an exception if an unknown database type is provided.
     *
     * @param databaseType The type of the database to load.
     * @throws std::runtime_error if the database type is unknown.
     */
    void loadDatabase(const std::string& databaseType);

    /**
     * @brief Sets up a MariaDB database connection using ConfigManager settings.
     *
     * @param config Reference to the ConfigManager instance.
     * @return std::shared_ptr<IDatabase> - Configured MariaDB database instance.
     */
   static std::shared_ptr<IDatabase> setupMariaDB(const ConfigManager& config);

    /**
     * @brief Sets up a PostgreSQL database connection using ConfigManager settings.
     *
     * @param config Reference to the ConfigManager instance.
     * @return std::shared_ptr<IDatabase> - Configured PostgreSQL database instance.
     */
    static std::shared_ptr<IDatabase> setupPostgreSQL(const ConfigManager& config);

    std::shared_ptr<IEncodingService> encodingService;  ///< Pointer to the encoding service instance.
    std::shared_ptr<IDatabase> database;                ///< Pointer to the database instance.
};
