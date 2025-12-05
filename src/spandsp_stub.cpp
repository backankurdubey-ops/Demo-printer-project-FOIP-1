#include "spandsp_stub.h"

bool spandsp_start_fax_session() {
    log_info("[SpanDSP] Initializing T.30 fax session");
    return true;
}

bool spandsp_encode_page(const std::vector<uint8_t>& page, 
                         std::vector<uint8_t>& outFrames) {

    log_info("[SpanDSP] Encoding fax page (" + std::to_string(page.size()) + " bytes)");

    outFrames.assign(10, 0xAA); // fake encoded data
    
    // Record frame metrics
    spandsp_record_frame(outFrames.size());
    
    return true;
}

bool spandsp_end_fax_session(std::vector<uint8_t>& finalFrames) {
    log_info("[SpanDSP] Finalizing fax session");

    finalFrames.assign(5, 0xFF); // fake termination frames
    
    // Record final frame metrics
    spandsp_record_frame(finalFrames.size());
    
    return true;
}

void spandsp_record_frame(size_t frameSize) {
    record_frame(frameSize);
}
