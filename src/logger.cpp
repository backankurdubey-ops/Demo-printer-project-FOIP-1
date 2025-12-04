#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

// Global metrics storage
static FoipMetrics g_metrics = {0, 0, 0, 0};
static std::chrono::steady_clock::time_point g_session_start;
static std::chrono::steady_clock::time_point g_session_end;

// Log level to string conversion
const char* log_level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Internal logging function
void log_message(LogLevel level, const std::string& module, const std::string& msg) {
    std::cout << "[" << log_level_to_string(level) << "] [" << module << "] " << msg << std::endl;
}

// Public logging API
void log_debug(const std::string& msg) {
    log_message(LogLevel::DEBUG, "Metrics", msg);
}

void log_info(const std::string& msg) {
    log_message(LogLevel::INFO, "APP", msg);
}

void log_warn(const std::string& msg) {
    log_message(LogLevel::WARN, "APP", msg);
}

void log_error(const std::string& msg) {
    log_message(LogLevel::ERROR, "APP", msg);
}

// Metrics API implementation
void record_frame(size_t frameSize) {
    g_metrics.totalFrames++;
    g_metrics.totalBytes += frameSize;
    log_message(LogLevel::DEBUG, "Metrics", "Recorded frame: " + std::to_string(frameSize) + " bytes");
}

void record_error() {
    g_metrics.errors++;
}

void start_session_timer() {
    g_session_start = std::chrono::steady_clock::now();
}

void end_session_timer() {
    g_session_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(g_session_end - g_session_start);
    g_metrics.sessionDurationMs = duration.count();
}

FoipMetrics get_foip_metrics() {
    return g_metrics;
}

void print_session_summary() {
    std::cout << "=== FOIP SESSION METRICS ===" << std::endl;
    std::cout << "Frames Sent: " << g_metrics.totalFrames << std::endl;
    std::cout << "Total Bytes: " << g_metrics.totalBytes << std::endl;
    std::cout << "Errors: " << g_metrics.errors << std::endl;
    std::cout << "Session Duration: " << g_metrics.sessionDurationMs << " ms" << std::endl;
    std::cout << "=== END ===" << std::endl;
}
