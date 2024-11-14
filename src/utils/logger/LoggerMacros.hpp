#include <absl/log/log.h>
#include <string>
#include <memory>
#include <absl/strings/str_format.h>

// Define log macros
#define LOG_TRACE(msg, ...) LOG(INFO).AtLocation(__FILE__, __LINE__) << "[TRACE] " << absl::StrFormat(msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) LOG(INFO).AtLocation(__FILE__, __LINE__) << "[DEBUG] " << absl::StrFormat(msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)  LOG(INFO).AtLocation(__FILE__, __LINE__) << "[INFO] " << absl::StrFormat(msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...)  LOG(WARNING).AtLocation(__FILE__, __LINE__) << "[WARN] " << absl::StrFormat(msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) LOG(ERROR).AtLocation(__FILE__, __LINE__) << "[ERROR] " << absl::StrFormat(msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...) LOG(FATAL).AtLocation(__FILE__, __LINE__) << "[FATAL] " << absl::StrFormat(msg, ##__VA_ARGS__)