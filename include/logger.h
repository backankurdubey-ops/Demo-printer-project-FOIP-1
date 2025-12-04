#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstddef>
#include <cstdint>

// Logging API
void log_debug(const std::string& msg);
void log_info(const std::string& msg);
void log_warn(const std::string& msg);
void log_error(const std::string& msg);

// Metrics structures
struct FoipMetrics {
    size_t totalFrames;
    size_t totalBytes;
    size_t errors;
};

// Metrics API
void record_frame(size_t frameSize);
void record_error();
FoipMetrics get_foip_metrics();

// Session timing
void start_session_timer();
void end_session_timer();
uint64_t get_session_duration_ms();

// Session summary
void print_session_summary();

#endif // LOGGER_H
