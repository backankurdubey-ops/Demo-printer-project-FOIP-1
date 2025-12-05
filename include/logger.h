#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstddef>
#include <cstdint>

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
    uint64_t sessionStartTimestamp;
    uint64_t sessionEndTimestamp;
};

// SIP Metrics structure
struct SipMetrics {
    size_t packetsSent;
    size_t bytesSent;
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
void start_session();
void end_session();

// Session metrics report
void print_session_metrics();

// Initialize logging system
void init_logger();

#endif
