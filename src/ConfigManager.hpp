#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <filesystem>
#include "Logger.hpp"

using json = nlohmann::json;

class ConfigManager {
public:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Load configuration from a specified file path
    void loadConfig(const std::string& configFilePath) {
        std::lock_guard<std::mutex> lock(configMutex);

        // Check if the file exists
        if (!std::filesystem::exists(configFilePath)) {
            Logger::getInstance().error("Configuration file {} does not exist.", configFilePath);
            throw std::runtime_error("Configuration file not found.");
        }

        try {
            std::ifstream configFile(configFilePath);
            if (!configFile.is_open()) {
                throw std::runtime_error("Failed to open configuration file.");
            }
            configFile >> config;
            Logger::getInstance().info("Configuration loaded successfully from {}", configFilePath);
        } catch (const std::exception& e) {
            Logger::getInstance().error("Error loading configuration: {}", e.what());
            throw;
        }
    }

    // Retrieve a configuration value with a default
    template <typename T>
    T get(const std::string& key, const T& defaultValue = T{}) const {
        std::lock_guard<std::mutex> lock(configMutex);

        // Check if key exists and return value or default
        if (config.contains(key)) {
            try {
                Logger::getInstance().debug("Retrieved configuration for key: {}", key);
                return config.at(key).get<T>();
            } catch (const std::exception& e) {
                Logger::getInstance().error("Error retrieving config key '{}': {}", key, e.what());
            }
        } else {
            Logger::getInstance().warn("Config key '{}' not found. Using default.", key);
        }
        return defaultValue;
    }

    // Update or add a configuration key-value pair at runtime
    template <typename T>
    void update(const std::string& key, const T& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        config[key] = value;
        Logger::getInstance().info("Configuration updated: {} -> {}", key, value);
    }

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    json config;
    mutable std::mutex configMutex;
};