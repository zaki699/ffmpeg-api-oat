#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <filesystem>
#include "Logger.hpp"
#include "logger/LoggerMacros.hpp"

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
            LOG_ERROR("Configuration file %s does not exist.", configFilePath);
            throw std::runtime_error("Configuration file not found.");
        }

        try {
            std::ifstream configFile(configFilePath);
            if (!configFile.is_open()) {
                throw std::runtime_error("Failed to open configuration file.");
            }
            configFile >> config;
            this->configFilePath = configFilePath;
            LOG_INFO("Configuration loaded successfully from %s", configFilePath);
        } catch (const std::exception& e) {
            LOG_ERROR("Error loading configuration: %s", e.what());
            throw;
        }
    }

    // Retrieve a configuration value with a default
    template <typename T>
    T get(const std::string& key, const T& defaultValue = T{}) const {
        std::lock_guard<std::mutex> lock(configMutex);

        if (config.contains(key)) {
            try {
                LOG_DEBUG("Retrieved configuration for key: %s", key);
                return config.at(key).get<T>();
            } catch (const nlohmann::json::type_error& e) {
                LOG_ERROR("Type mismatch for key '%s': %s", key, e.what());
            } catch (const std::exception& e) {
                LOG_ERROR("Error retrieving config key '%s': %s", key, e.what());
            }
        } else {
            LOG_WARN("Config key '%s' not found. Using default.", key);
        }
        return defaultValue;
    }

    // Update or add a configuration key-value pair at runtime
    template <typename T>
    void update(const std::string& key, const T& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        config[key] = value;
        LOG_INFO("Configuration updated: %s -> %s", key, value);
        saveConfig();
    }

    // Save the current configuration to file
    void saveConfig() const {
        if (configFilePath.empty()) {
            LOG_WARN("Configuration file path is empty; cannot save.");
            return;
        }

        std::lock_guard<std::mutex> lock(configMutex);
        try {
            std::ofstream configFile(configFilePath);
            if (!configFile.is_open()) {
                throw std::runtime_error("Failed to open configuration file for writing.");
            }
            configFile << config.dump(4); // Pretty print with 4 spaces
            LOG_INFO("Configuration saved to %s", configFilePath);
        } catch (const std::exception& e) {
            LOG_ERROR("Error saving configuration: %s", e.what());
        }
    }

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    mutable std::mutex configMutex;
    json config;
    std::string configFilePath; // Store the path for saving updated configs
};