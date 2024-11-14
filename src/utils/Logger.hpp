#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <absl/log/log.h>
#include <absl/log/log_sink.h>
#include <absl/strings/str_cat.h>
#include <absl/log/log_sink_registry.h>
#include <string>
#include <memory>
#include <fstream>
#include <mutex>

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
    // Disable copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Singleton instance
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // Set log file path (Optional)
    void setLogFilePath(const std::string& filePath) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!fileSink_ || filePath != logFilePath_) {
            if (fileSink_) {
                absl::RemoveLogSink(fileSink_.get());
            }
            fileSink_ = std::make_unique<FileSink>(filePath);
            absl::AddLogSink(fileSink_.get());
            logFilePath_ = filePath;
        }
    }

    // Set the minimum log level
    void setLogLevel(const LogLevel level) {
        minLogLevel_ = level;
    }

    // Log a message with a specific level
    template <typename... Args>
    void log(const LogLevel level, const Args&... args) {
        if (level < minLogLevel_) return;

        const std::string message = absl::StrCat(args...);
        switch (level) {
            case LogLevel::TRACE:
                LOG(INFO) << "[TRACE] " << message;
                break;
            case LogLevel::DEBUG:
                LOG(INFO) << "[DEBUG] " << message;
                break;
            case LogLevel::INFO:
                LOG(INFO) << "[INFO] " << message;
                break;
            case LogLevel::WARN:
                LOG(WARNING) << "[WARN] " << message;
                break;
            case LogLevel::ERROR:
                LOG(ERROR) << "[ERROR] " << message;
                break;
            case LogLevel::FATAL:
                LOG(FATAL) << "[FATAL] " << message;
                break;
        }
    }

    // Convenience methods for different log levels
    template <typename... Args>
    void trace(const Args&... args) { log(LogLevel::TRACE, args...); }
    template <typename... Args>
    void debug(const Args&... args) { log(LogLevel::DEBUG, args...); }
    template <typename... Args>
    void info(const Args&... args) { log(LogLevel::INFO, args...); }
    template <typename... Args>
    void warn(const Args&... args) { log(LogLevel::WARN, args...); }
    template <typename... Args>
    void error(const Args&... args) { log(LogLevel::ERROR, args...); }
    template <typename... Args>
    void fatal(const Args&... args) { log(LogLevel::FATAL, args...); }

private:
    LogLevel minLogLevel_ = LogLevel::INFO;
    std::string logFilePath_;
    std::unique_ptr<absl::LogSink> fileSink_;
    std::mutex mutex_; // Ensure thread-safe log file handling

    Logger() = default;

    class FileSink final : public absl::LogSink {
    public:
        explicit FileSink(const std::string& filePath) {
            fileStream_.open(filePath, std::ios::app);
            if (!fileStream_) {
                throw std::runtime_error("Unable to open log file: " + filePath);
            }
        }

        void Send(const absl::LogEntry& entry) override {
            if (fileStream_.is_open()) {
                fileStream_ << entry.text_message() << std::endl;
            }
        }

        ~FileSink() override {
            if (fileStream_.is_open()) {
                fileStream_.close();
            }
        }

    private:
        std::ofstream fileStream_;
    };
};

#endif // LOGGER_HPP