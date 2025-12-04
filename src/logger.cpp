#include "../include/logger.h"
#include <iomanip>
#include <sstream>

// Global metrics storage
static FoipMetrics g_metrics = {0, 0, 0};
static std::chrono::steady_clock::time_point g_session_start;
static std::chrono::steady_clock::time_point g_session_end;
static bool g_session_active = false;

// Logging implementation
void log_debug(const std::string& module, const std::string& msg) {
    std::cout << "[DEBUG] [" << module << "] " << msg << std::endl;
}

void log_info(const std::string& module, const std::string& msg) {
    std::cout << "[INFO] [" << module << "] " << msg << std::endl;
}

void log_warn(const std::string& module, const std::string& msg) {
    std::cout << "[WARN] [" << module << "] " << msg << std::endl;
}

void log_error(const std::string& module, const std::string& msg) {
    std::cout << "[ERROR] [" << module << "] " << msg << std::endl;
}

// Metrics implementation
void record_frame(size_t frameSize) {
    g_metrics.totalFrames++;
    g_metrics.totalBytes += frameSize;
    
    std::stringstream ss;
    ss << "Recorded frame: " << frameSize << " bytes";
    log_debug("Metrics", ss.str());
}

void record_error() {
    g_metrics.errors++;
}

FoipMetrics get_foip_metrics() {
    return g_metrics;
}

void reset_metrics() {
    g_metrics = {0, 0, 0};
}

// Session timing implementation
void start_session_timer() {
    g_session_start = std::chrono::steady_clock::now();
    g_session_active = true;
}

void end_session_timer() {
    if (g_session_active) {
        g_session_end = std::chrono::steady_clock::now();
        g_session_active = false;
    }
}

uint64_t get_session_duration_ms() {
    if (g_session_active) {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - g_session_start).count();
    } else {
        return std::chrono::duration_cast<std::chrono::milliseconds>(g_session_end - g_session_start).count();
    }
}
