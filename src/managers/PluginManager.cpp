#include "PluginManager.hpp"
#include "ConfigManager.hpp"
#include "Logger.hpp"
#include "SQLiteDatabase.hpp"
#include "MariaDBDatabase.hpp"
#include "AWSS3Provider.hpp"
#include "FFmpegEncodingService.hpp"

// Singleton instance
PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

// Getters for the plugins
std::shared_ptr<IStorageProvider> PluginManager::getStorageProvider() {
    return storageProvider;
}

std::shared_ptr<IEncodingService> PluginManager::getEncodingService() {
    return encodingService;
}

std::shared_ptr<IDatabase> PluginManager::getDatabase() {
    return database;
}

// Initialize all plugins based on configuration
void PluginManager::initialize() {
    auto config = ConfigManager::getInstance();

    // Load storage provider
    std::string storageProviderType = config.get("storage_provider").get<std::string>();
    loadStorageProvider(storageProviderType);

    // Load encoding service
    std::string encodingServiceType = config.get("encoding_service").get<std::string>();
    loadEncodingService(encodingServiceType);

    // Load database
    std::string databaseType = config.get("database_type").get<std::string>();
    loadDatabase(databaseType);
}

// Load methods
void PluginManager::loadStorageProvider(const std::string& providerName) {
    Logger::getInstance().info("Loading storage provider: {}", providerName);

    if (providerName == "aws_s3") {
        storageProvider = std::make_shared<AWSS3Provider>();
    }
    // Add more providers as needed
    else {
        Logger::getInstance().error("Unknown storage provider: {}", providerName);
        throw std::runtime_error("Failed to load storage provider: " + providerName);
    }
}

void PluginManager::loadEncodingService(const std::string& serviceName) {
    Logger::getInstance().info("Loading encoding service: {}", serviceName);

    if (serviceName == "ffmpeg") {
        encodingService = std::make_shared<FFmpegEncodingService>();
    }
    // Add more encoding services as needed
    else {
        Logger::getInstance().error("Unknown encoding service: {}", serviceName);
        throw std::runtime_error("Failed to load encoding service: " + serviceName);
    }
}

void PluginManager::loadDatabase(const std::string& databaseType) {
    Logger::getInstance().info("Loading database: {}", databaseType);

    if (databaseType == "sqlite") {
        std::string dbPath = ConfigManager::getInstance().get("database_path").get<std::string>();
        database = std::make_shared<SQLiteDatabase>(dbPath);
    }
    else if (databaseType == "mariadb") {
        auto mariadbConfig = ConfigManager::getInstance().get("mariadb");

        std::string host = mariadbConfig["host"].get<std::string>();
        int port = mariadbConfig["port"].get<int>();
        std::string username = mariadbConfig["username"].get<std::string>();
        std::string password = mariadbConfig["password"].get<std::string>();
        std::string databaseName = mariadbConfig["database"].get<std::string>();

        database = std::make_shared<MariaDBDatabase>(host, port, username, password, databaseName);
    }
    else {
        Logger::getInstance().error("Unknown database type: {}", databaseType);
        throw std::runtime_error("Failed to load database: " + databaseType);
    }
}