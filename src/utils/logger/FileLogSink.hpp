#ifndef FILE_LOG_SINK_HPP
#define FILE_LOG_SINK_HPP

#include <absl/log/log_sink.h>
#include <string>
#include <fstream>
#include <mutex>

class FileLogSink : public absl::LogSink {
public:
    explicit FileLogSink(const std::string& filePath);
    void Send(const absl::LogEntry& entry) override;
    ~FileLogSink() override;

private:
    std::ofstream fileStream_;
    std::mutex mutex_;
};

#endif // FILE_LOG_SINK_HPP