#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

// Global metrics storage
static FoipMetrics g_metrics = {0, 0, 0};
static std::chrono::steady_clock::time_point g_session_start;
static std::chrono::steady_clock::time_point g_session_end;
static bool g_session_active = false;

// Logging functions
void log_debug(const std::string& msg) {
    std::cout << "[DEBUG] " << msg << std::endl;
}

void log_info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

void log_warn(const std::string& msg) {
    std::cout << "[WARN] " << msg << std::endl;
}

void log_error(const std::string& msg) {
    std::cout << "[ERROR] " << msg << std::endl;
}

// Metrics functions
void record_frame(size_t frameSize) {
    g_metrics.totalFrames++;
    g_metrics.totalBytes += frameSize;
    log_debug("[Metrics] Recorded frame: " + std::to_string(frameSize) + " bytes");
}

void record_error() {
    g_metrics.errors++;
}

FoipMetrics get_foip_metrics() {
    return g_metrics;
}

// Session timing functions
void start_session_timer() {
    g_session_start = std::chrono::steady_clock::now();
    g_session_active = true;
}

void end_session_timer() {
    g_session_end = std::chrono::steady_clock::now();
    g_session_active = false;
}

uint64_t get_session_duration_ms() {
    if (g_session_active) {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - g_session_start).count();
    } else {
        return std::chrono::duration_cast<std::chrono::milliseconds>(g_session_end - g_session_start).count();
    }
}

void print_session_summary() {
    std::cout << "\n=== FOIP SESSION METRICS ===" << std::endl;
    std::cout << "Frames Sent: " << g_metrics.totalFrames << std::endl;
    std::cout << "Total Bytes: " << g_metrics.totalBytes << std::endl;
    std::cout << "Errors: " << g_metrics.errors << std::endl;
    std::cout << "Session Duration: " << get_session_duration_ms() << " ms" << std::endl;
    std::cout << "=== END ===" << std::endl;
}
