#include "PluginManager.hpp"

#include <utils/logger/LoggerMacros.hpp>
#include "utils/ConfigManager.hpp"
#include "utils/Logger.hpp"
#include "database/sqlite/SQLiteDatabase.hpp"
#include "database/mariadb/MariaDBDatabase.hpp"
#include "database/postgresql/PostgreSQLDatabase.hpp"
#include "providers/AWSS3Provider.hpp"
#include "encoding/FFmpegEncodingService.hpp"

PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

std::shared_ptr<IEncodingService> PluginManager::getEncodingService() const {
    return encodingService;
}

std::shared_ptr<IDatabase> PluginManager::getDatabase() const {
    return database;
}

void PluginManager::initialize() {
    LOG_INFO("PluginManager starting initialization.");
    const auto& config = ConfigManager::getInstance();

    loadEncodingService(config.get<std::string>("encoding_service", "ffmpeg"));
    loadDatabase(config.get<std::string>("database_type", "sqlite"));

    if (database) {
        database->connect();
    }

    LOG_INFO("PluginManager initialization complete.");
}

void PluginManager::shutdown() const {
    if (database) {
        database->disconnect();
    }
    LOG_INFO("PluginManager shutdown complete.");
}

void PluginManager::loadEncodingService(const std::string& serviceName) {
    LOG_INFO("Loading encoding service: %s", serviceName);

    if (serviceName == "ffmpeg") {
        encodingService = std::make_shared<FFmpegEncodingService>();
    } else {
        throw std::runtime_error("Unknown encoding service: " + serviceName);
    }
}

void PluginManager::loadDatabase(const std::string& databaseType) {
    LOG_INFO("Loading database: %s", databaseType);

    const auto& config = ConfigManager::getInstance();

    if (databaseType == "sqlite") {
        database = std::make_shared<SQLiteDatabase>(config.get<std::string>("database.sqlite.path", "default.db"));
    } else if (databaseType == "mariadb") {
        database = setupMariaDB(config);
    } else if (databaseType == "postgresql") {
        database = setupPostgreSQL(config);
    } else {
        throw std::runtime_error("Unknown database type: " + databaseType);
    }
}

 std::shared_ptr<IDatabase> PluginManager::setupMariaDB(const ConfigManager& config) {
    auto poolSize = config.get<size_t>("database.mariadb.pool_size", 10);
    auto host = config.get<std::string>("database.mariadb.host", "localhost");
    auto user = config.get<std::string>("database.mariadb.user", "root");
    auto password = config.get<std::string>("database.mariadb.password", "");
    auto dbname = config.get<std::string>("database.mariadb.database", "mydb");
    auto port = config.get<unsigned int>("database.mariadb.port", 3306);

    return std::make_shared<MariaDBDatabase>(host, user, password, dbname, port, poolSize);
}

 std::shared_ptr<IDatabase> PluginManager::setupPostgreSQL(const ConfigManager& config) {
    auto poolSize = config.get<size_t>("database.postgresql.pool_size", 10);
    auto connInfo = config.get<std::string>("database.postgresql.conninfo", "dbname=mydb");

    return std::make_shared<PostgreSQLDatabase>(connInfo, poolSize);
}