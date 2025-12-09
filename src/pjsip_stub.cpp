#include "pjsip_stub.h"
#include "logger.h"
#include <chrono>

// Global SIP metrics
static SipMetrics g_sip_metrics = {0, 0, 0};
static std::chrono::steady_clock::time_point g_sip_session_start;

bool pjsip_make_call(const std::string& number) {
    log_info("PJSIP", "Making SIP call to " + number);
    
    // Start session timing
    g_sip_session_start = std::chrono::steady_clock::now();
    
    // Simulate call setup
    if (number.empty()) {
        log_error("PJSIP", "Call failed: empty number");
        record_error("PJSIP");
        return false;
    }
    
    log_info("PJSIP", "SIP call established successfully");
    return true;
}

bool pjsip_send_udptl(const uint8_t* data, size_t len) {
    if (!data || len == 0) {
        log_warn("PJSIP", "UDPTL send failed: invalid data");
        record_error("PJSIP");
        return false;
    }
    
    log_info("PJSIP", "Sending UDPTL packet (size=" + std::to_string(len) + ")");
    
    // Update SIP metrics
    g_sip_metrics.packetsSent++;
    g_sip_metrics.bytesSent += len;
    
    // Record frame in global metrics
    record_frame("PJSIP", len);
    
    return true;
}

bool pjsip_hangup() {
    log_info("PJSIP", "Hanging up SIP call");
    
    // Calculate session duration
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - g_sip_session_start);
    g_sip_metrics.sessionDurationMs = duration.count();
    
    log_info("PJSIP", "SIP call terminated");
    return true;
}

SipMetrics pjsip_get_session_metrics() {
    return g_sip_metrics;
}

void pjsip_reset_metrics() {
    g_sip_metrics = {0, 0, 0};
}