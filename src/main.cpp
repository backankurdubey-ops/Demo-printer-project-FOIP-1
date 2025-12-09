#include <iostream>
#include <vector>
#include "pjsip_stub.h"
#include "spandsp_stub.h"
#include "logger.h"

void print_session_metrics() {
    FoipMetrics metrics = get_foip_metrics();
    SipMetrics sipMetrics = pjsip_get_session_metrics();
    
    std::cout << "\n=== FOIP SESSION METRICS ===" << std::endl;
    std::cout << "Frames Sent: " << metrics.totalFrames << std::endl;
    std::cout << "Total Bytes: " << metrics.totalBytes << std::endl;
    std::cout << "Errors: " << metrics.errors << std::endl;
    std::cout << "Session Duration: " << metrics.sessionDurationMs << " ms" << std::endl;
    std::cout << "SIP Packets Sent: " << sipMetrics.packetsSent << std::endl;
    std::cout << "SIP Bytes Sent: " << sipMetrics.bytesSent << std::endl;
    std::cout << "=== END ===" << std::endl;
}

int main() {
    std::string number = "1234567890";

    // Initialize metrics and logging
    reset_metrics();
    pjsip_reset_metrics();
    
    log_info("APP", "=== DIRECT FOIP STACK CALL DEMO ===");
    start_session_timer();

    // 1. PJSIP: start SIP call
    log_info("APP", "Step 1: Establishing SIP call");
    if (!pjsip_make_call(number)) {
        log_error("APP", "Failed to make SIP call");
        record_error("APP");
        return 1;
    }

    // 2. SpanDSP: start fax session
    log_info("APP", "Step 2: Starting fax session");
    if (!spandsp_start_fax_session()) {
        log_error("APP", "Failed to start fax session");
        record_error("APP");
        return 1;
    }

    // Example: single fake TIFF page (raw bytes)
    std::vector<uint8_t> page = { 1, 2, 3, 4, 5 };
    std::vector<uint8_t> encoded;

    // 3. Encode page using SpanDSP T.30 logic
    log_info("APP", "Step 3: Encoding fax page");
    if (!spandsp_encode_page(page, encoded)) {
        log_error("APP", "Failed to encode fax page");
        record_error("APP");
        return 1;
    }

    // 4. Send encoded frames via PJSIP UDPTL
    log_info("APP", "Step 4: Sending encoded frames");
    if (!pjsip_send_udptl(encoded.data(), encoded.size())) {
        log_error("APP", "Failed to send UDPTL packet");
        record_error("APP");
        return 1;
    }

    // 5. End fax session
    log_info("APP", "Step 5: Finalizing fax session");
    std::vector<uint8_t> finalFrames;
    if (!spandsp_end_fax_session(finalFrames)) {
        log_error("APP", "Failed to end fax session");
        record_error("APP");
        return 1;
    }

    // 6. Send final frames
    log_info("APP", "Step 6: Sending final frames");
    if (!pjsip_send_udptl(finalFrames.data(), finalFrames.size())) {
        log_warn("APP", "Failed to send final UDPTL frames");
        record_error("APP");
    }

    // 7. Hangup SIP call
    log_info("APP", "Step 7: Terminating SIP call");
    if (!pjsip_hangup()) {
        log_error("APP", "Failed to hangup SIP call");
        record_error("APP");
    }

    // End session timing
    end_session_timer();

    log_info("APP", "=== FAX SEND COMPLETE ===");
    
    // Print comprehensive metrics report
    print_session_metrics();

    return 0;
}