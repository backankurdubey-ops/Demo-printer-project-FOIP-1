#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <iostream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

struct FoipMetrics {
    size_t totalFrames;
    size_t totalBytes;
    size_t errors;
    uint64_t sessionDurationMs;
};

struct SipMetrics {
    size_t packetsSent;
    size_t bytesSent;
    uint64_t sessionDurationMs;
};

class Logger {
private:
    static FoipMetrics metrics;
    static std::chrono::steady_clock::time_point sessionStart;
    static bool sessionStarted;

public:
    static void log_debug(const std::string& msg);
    static void log_info(const std::string& msg);
    static void log_warn(const std::string& msg);
    static void log_error(const std::string& msg);
    
    static void record_frame(size_t frameSize);
    static void record_error();
    static FoipMetrics get_foip_metrics();
    
    static void start_session();
    static void end_session();
    static void print_session_summary();

private:
    static void log(LogLevel level, const std::string& msg);
    static std::string level_to_string(LogLevel level);
};

#endif
