#include "spandsp_stub.h"

bool spandsp_start_fax_session() {
    std::cout << "[SpanDSP] Initializing T.30 fax session" << std::endl;
    return true;
}

bool spandsp_encode_page(const std::vector<uint8_t>& page, 
                         std::vector<uint8_t>& outFrames) {

    std::cout << "[SpanDSP] Encoding fax page (" 
              << page.size() << " bytes)" << std::endl;

    outFrames.assign(10, 0xAA); // fake encoded data
    return true;
}

bool spandsp_end_fax_session(std::vector<uint8_t>& finalFrames) {
    std::cout << "[SpanDSP] Finalizing fax session" << std::endl;

    finalFrames.assign(5, 0xFF); // fake termination frames
    return true;
}
