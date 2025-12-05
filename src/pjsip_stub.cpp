#include "pjsip_stub.h"

// Global SIP metrics
static SipMetrics g_sip_metrics = {0, 0, 0};

bool pjsip_make_call(const std::string& number) {
    log_info("[PJSIP] Making SIP call to " + number);
    return true;
}

bool pjsip_send_udptl(const uint8_t* data, size_t len) {
    log_info("[PJSIP] Sending UDPTL packet (size=" + std::to_string(len) + ")");
    
    // Track metrics
    g_sip_metrics.packetsSent++;
    g_sip_metrics.bytesSent += len;
    
    // Record frame in global metrics
    record_frame(len);
    
    return true;
}

bool pjsip_hangup() {
    log_info("[PJSIP] Hanging up SIP call");
    return true;
}

SipMetrics pjsip_get_session_metrics() {
    FoipMetrics foip_metrics = get_foip_metrics();
    g_sip_metrics.sessionDurationMs = foip_metrics.sessionEndTimestamp - foip_metrics.sessionStartTimestamp;
    return g_sip_metrics;
}
