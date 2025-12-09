#include "logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>

// Global metrics storage
static FoipMetrics g_metrics = {0, 0, 0, 0};
static std::chrono::steady_clock::time_point g_session_start;
static std::chrono::steady_clock::time_point g_session_end;
static std::mutex g_metrics_mutex;
static LogLevel g_current_log_level = LogLevel::INFO;

// Convert log level to string
std::string log_level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Get current timestamp string
std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

// Internal logging function
void log_message(LogLevel level, const std::string& module, const std::string& message) {
    if (level < g_current_log_level) {
        return;
    }
    
    std::cout << "[" << get_timestamp() << "] "
              << "[" << log_level_to_string(level) << "] "
              << "[" << module << "] "
              << message << std::endl;
}

// Logging API implementations
void log_debug(const std::string& module, const std::string& message) {
    log_message(LogLevel::DEBUG, module, message);
}

void log_info(const std::string& module, const std::string& message) {
    log_message(LogLevel::INFO, module, message);
}

void log_warn(const std::string& module, const std::string& message) {
    log_message(LogLevel::WARN, module, message);
}

void log_error(const std::string& module, const std::string& message) {
    log_message(LogLevel::ERROR, module, message);
}

// Metrics API implementations
void record_frame(const std::string& module, size_t frameSize) {
    std::lock_guard<std::mutex> lock(g_metrics_mutex);
    g_metrics.totalFrames++;
    g_metrics.totalBytes += frameSize;
    
    log_debug("Metrics", "Recorded frame: " + std::to_string(frameSize) + " bytes from " + module);
}

void record_error(const std::string& module) {
    std::lock_guard<std::mutex> lock(g_metrics_mutex);
    g_metrics.errors++;
    
    log_debug("Metrics", "Recorded error from " + module);
}

FoipMetrics get_foip_metrics() {
    std::lock_guard<std::mutex> lock(g_metrics_mutex);
    return g_metrics;
}

void reset_metrics() {
    std::lock_guard<std::mutex> lock(g_metrics_mutex);
    g_metrics = {0, 0, 0, 0};
}

void start_session_timer() {
    g_session_start = std::chrono::steady_clock::now();
}

void end_session_timer() {
    g_session_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        g_session_end - g_session_start);
    
    std::lock_guard<std::mutex> lock(g_metrics_mutex);
    g_metrics.sessionDurationMs = duration.count();
}