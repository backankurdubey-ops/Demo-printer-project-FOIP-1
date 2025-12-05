#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

// Global metrics storage
static FoipMetrics g_metrics = {0, 0, 0, 0, 0};

// Helper function to get current timestamp
static uint64_t get_current_timestamp() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

// Helper function to format log message
static void log_message(const std::string& level, const std::string& module, const std::string& msg) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::cout << "[" << level << "] [" << module << "] " << msg << std::endl;
}

void init_logger() {
    // Initialize logging system
    g_metrics = {0, 0, 0, 0, 0};
}

void log_debug(const std::string& msg) {
    log_message("DEBUG", "APP", msg);
}

void log_info(const std::string& msg) {
    log_message("INFO", "APP", msg);
}

void log_warn(const std::string& msg) {
    log_message("WARN", "APP", msg);
}

void log_error(const std::string& msg) {
    log_message("ERROR", "APP", msg);
    record_error();
}

void record_frame(size_t frameSize) {
    g_metrics.totalFrames++;
    g_metrics.totalBytes += frameSize;
    log_message("DEBUG", "Metrics", "Recorded frame: " + std::to_string(frameSize) + " bytes");
}

void record_error() {
    g_metrics.errors++;
}

FoipMetrics get_foip_metrics() {
    return g_metrics;
}

void start_session() {
    g_metrics.sessionStartTimestamp = get_current_timestamp();
    log_message("INFO", "APP", "FOIP session started");
}

void end_session() {
    g_metrics.sessionEndTimestamp = get_current_timestamp();
    log_message("INFO", "APP", "FOIP session ended");
}

void print_session_metrics() {
    uint64_t duration = g_metrics.sessionEndTimestamp - g_metrics.sessionStartTimestamp;
    
    std::cout << "\n=== FOIP SESSION METRICS ===" << std::endl;
    std::cout << "Frames Sent: " << g_metrics.totalFrames << std::endl;
    std::cout << "Total Bytes: " << g_metrics.totalBytes << std::endl;
    std::cout << "Errors: " << g_metrics.errors << std::endl;
    std::cout << "Session Duration: " << duration << " ms" << std::endl;
    std::cout << "=== END ===" << std::endl;
}
