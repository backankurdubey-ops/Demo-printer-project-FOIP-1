#include <iostream>
#include <vector>
#include "pjsip_stub.h"
#include "spandsp_stub.h"

int main() {
    std::string number = "1234567890";

    std::cout << "=== DIRECT FOIP STACK CALL DEMO ===\n";

    // 1. PJSIP: start SIP call
    if (!pjsip_make_call(number)) {
        std::cerr << "Failed to make SIP call\n";
        return 1;
    }

    // 2. SpanDSP: start fax session
    spandsp_start_fax_session();

    // Example: single fake TIFF page (raw bytes)
    std::vector<uint8_t> page = { 1, 2, 3, 4, 5 };
    std::vector<uint8_t> encoded;

    // 3. Encode page using SpanDSP T.30 logic
    spandsp_encode_page(page, encoded);

    // 4. Send encoded frames via PJSIP UDPTL
    pjsip_send_udptl(encoded.data(), encoded.size());

    // 5. End fax session
    std::vector<uint8_t> finalFrames;
    spandsp_end_fax_session(finalFrames);

    // 6. Send final frames
    pjsip_send_udptl(finalFrames.data(), finalFrames.size());

    // 7. Hangup SIP call
    pjsip_hangup();

    std::cout << "=== FAX SEND COMPLETE ===\n";
    //new changes for branch
    return 0;
}
