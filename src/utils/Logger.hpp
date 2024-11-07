#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>
#include <ctime>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
public:
    // Singleton instance
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // Set log file path (Optional)
    void setLogFilePath(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (logFile_.is_open()) {
            logFile_.close();
        }
        logFile_.open(filePath, std::ios::app);
    }

    // Set the minimum log level
    void setLogLevel(LogLevel level) {
        minLogLevel_ = level;
    }

    // Log a message with a specific level
    void log(LogLevel level, const std::string& message) {
        if (level < minLogLevel_) return;

        std::lock_guard<std::mutex> lock(mutex_);
        std::string logMessage = formatLogMessage(level, message);

        // Write to console
        std::cout << logMessage << std::endl;

        // Write to log file if configured
        if (logFile_.is_open()) {
            logFile_ << logMessage << std::endl;
        }
    }

    // Convenience methods for different log levels
    void trace(const std::string& message) { log(LogLevel::TRACE, message); }
    void debug(const std::string& message) { log(LogLevel::DEBUG, message); }
    void info(const std::string& message) { log(LogLevel::INFO, message); }
    void warn(const std::string& message) { log(LogLevel::WARN, message); }
    void error(const std::string& message) { log(LogLevel::ERROR, message); }
    void fatal(const std::string& message) { log(LogLevel::FATAL, message); }

private:
    LogLevel minLogLevel_ = LogLevel::INFO;      // Default log level
    std::ofstream logFile_;                      // Log file stream
    std::mutex mutex_;                           // Mutex for thread safety

    Logger() = default;
    ~Logger() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    // Disable copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Helper function to format log message with timestamp and level
    std::string formatLogMessage(LogLevel level, const std::string& message) {
        const char* levelStr;
        switch (level) {
            case LogLevel::TRACE: levelStr = "TRACE"; break;
            case LogLevel::DEBUG: levelStr = "DEBUG"; break;
            case LogLevel::INFO: levelStr = "INFO"; break;
            case LogLevel::WARN: levelStr = "WARN"; break;
            case LogLevel::ERROR: levelStr = "ERROR"; break;
            case LogLevel::FATAL: levelStr = "FATAL"; break;
            default: levelStr = "UNKNOWN"; break;
        }

        // Get current time
        std::time_t now = std::time(nullptr);
        char timeStr[20];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        // Format: [time] [level] message
        return std::string("[") + timeStr + "] [" + levelStr + "] " + message;
    }
};

#endif // LOGGER_HPP