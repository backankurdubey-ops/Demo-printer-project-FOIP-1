#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstdint>
#include <cstddef>

// Log levels
enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

// FOIP Metrics structure
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

// Module-specific logging
void log_debug_module(const std::string& module, const std::string& msg);
void log_info_module(const std::string& module, const std::string& msg);
void log_warn_module(const std::string& module, const std::string& msg);
void log_error_module(const std::string& module, const std::string& msg);

// Metrics API
void record_frame(size_t frameSize);
void record_error();
void reset_metrics();
FoipMetrics get_foip_metrics();

// Session timing
void start_session_timer();
void end_session_timer();

// Initialize/cleanup
void init_logger();
void cleanup_logger();

#endif // LOGGER_H
