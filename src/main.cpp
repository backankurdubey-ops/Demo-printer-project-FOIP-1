#include <iostream>
#include <vector>
#include "pjsip_stub.h"
#include "spandsp_stub.h"
#include "logger.h"

int main() {
    // Initialize logging system
    init_logger();
    
    std::string number = "1234567890";

    std::cout << "=== DIRECT FOIP STACK CALL DEMO ===\n";

    // Start session timing
    start_session();

    // 1. PJSIP: start SIP call
    if (!pjsip_make_call(number)) {
        log_error("[PJSIP] Call failed");
        return 1;
    }

    // 2. SpanDSP: start fax session
    if (!spandsp_start_fax_session()) {
        log_error("[SpanDSP] Fax session initialization failed");
        return 1;
    }

    // Example: single fake TIFF page (raw bytes)
    std::vector<uint8_t> page = { 1, 2, 3, 4, 5 };
    std::vector<uint8_t> encoded;

    // 3. Encode page using SpanDSP T.30 logic
    if (!spandsp_encode_page(page, encoded)) {
        log_error("[SpanDSP] Encoding failed");
        return 1;
    }

    // 4. Send encoded frames via PJSIP UDPTL
    if (!pjsip_send_udptl(encoded.data(), encoded.size())) {
        log_warn("[PJSIP] Packet send failed");
    }

    // 5. End fax session
    std::vector<uint8_t> finalFrames;
    if (!spandsp_end_fax_session(finalFrames)) {
        log_error("[SpanDSP] Session termination failed");
        return 1;
    }

    // 6. Send final frames
    if (!pjsip_send_udptl(finalFrames.data(), finalFrames.size())) {
        log_warn("[PJSIP] Final packet send failed");
    }

    // 7. Hangup SIP call
    if (!pjsip_hangup()) {
        log_error("[PJSIP] Hangup failed");
    }

    // End session timing
    end_session();

    std::cout << "=== FAX SEND COMPLETE ===\n";

    // Print session metrics report
    print_session_metrics();

    return 0;
}
