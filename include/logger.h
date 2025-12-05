#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstdint>
#include <chrono>

// Log levels
enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

// FOIP session metrics structure
struct FoipMetrics {
    size_t totalFrames;
    size_t totalBytes;
    size_t errors;
    uint64_t sessionDurationMs;
};

// Logging API
void log_debug(const std::string& msg);
void log_info(const std::string& msg);
void log_warn(const std::string& msg);
void log_error(const std::string& msg);

// Metrics API
void record_frame(size_t frameSize);
void record_error();
FoipMetrics get_foip_metrics();
void reset_metrics();
void start_session_timer();
void stop_session_timer();

#endif
