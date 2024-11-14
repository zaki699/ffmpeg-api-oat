// src/logger/FileLogSink.cpp
#include "FileLogSink.hpp"
#include <stdexcept>

FileLogSink::FileLogSink(const std::string& filePath) {
    fileStream_.open(filePath, std::ios::app);
    if (!fileStream_) {
        throw std::runtime_error("Unable to open log file: " + filePath);
    }
}

void FileLogSink::Send(const absl::LogEntry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    fileStream_ << entry.text_message() << std::endl;
}

FileLogSink::~FileLogSink() {
    if (fileStream_.is_open()) {
        fileStream_.close();
    }
}