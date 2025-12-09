#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstdint>
#include <chrono>

// Log levels
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
};

// FOIP session metrics structure
struct FoipMetrics {
    size_t totalFrames;
    size_t totalBytes;
    size_t errors;
    uint64_t sessionDurationMs;
};

// Logging API
void log_debug(const std::string& module, const std::string& message);
void log_info(const std::string& module, const std::string& message);
void log_warn(const std::string& module, const std::string& message);
void log_error(const std::string& module, const std::string& message);

// Metrics API
void record_frame(const std::string& module, size_t frameSize);
void record_error(const std::string& module);
FoipMetrics get_foip_metrics();
void reset_metrics();
void start_session_timer();
void end_session_timer();

// Internal logging function
void log_message(LogLevel level, const std::string& module, const std::string& message);

#endif