#pragma once
#include "IStorageProvider.hpp"
#include "IEncodingService.hpp"
#include "IDatabase.hpp"
#include <memory>
#include <string>

class PluginManager {
public:
    static PluginManager& getInstance();

    std::shared_ptr<IStorageProvider> getStorageProvider();
    std::shared_ptr<IEncodingService> getEncodingService();
    std::shared_ptr<IDatabase> getDatabase();

    void initialize(); // Initializes all plugins based on configuration

private:
    PluginManager() = default;

    void loadStorageProvider(const std::string& providerName);
    void loadEncodingService(const std::string& serviceName);
    void loadDatabase(const std::string& databaseType);

    std::shared_ptr<IStorageProvider> storageProvider;
    std::shared_ptr<IEncodingService> encodingService;
    std::shared_ptr<IDatabase> database;
};