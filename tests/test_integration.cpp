#include <iostream>
#include <cassert>
#include "../include/pjsip_stub.h"
#include "../include/spandsp_stub.h"
#include "../include/logger.h"

void test_end_to_end_workflow() {
    std::cout << "Testing end-to-end FOIP workflow..." << std::endl;
    
    // Reset all metrics
    reset_metrics();
    pjsip_reset_metrics();
    
    // Start session timer
    start_session_timer();
    
    // Test workflow
    std::string number = "1234567890";
    assert(pjsip_make_call(number) == true);
    assert(spandsp_start_fax_session() == true);
    
    std::vector<uint8_t> page = {1, 2, 3, 4, 5};
    std::vector<uint8_t> encoded;
    assert(spandsp_encode_page(page, encoded) == true);
    assert(pjsip_send_udptl(encoded.data(), encoded.size()) == true);
    
    std::vector<uint8_t> finalFrames;
    assert(spandsp_end_fax_session(finalFrames) == true);
    assert(pjsip_send_udptl(finalFrames.data(), finalFrames.size()) == true);
    assert(pjsip_hangup() == true);
    
    // End session timer
    end_session_timer();
    
    // Verify metrics
    FoipMetrics metrics = get_foip_metrics();
    assert(metrics.totalFrames == 2); // encoded + final frames
    assert(metrics.totalBytes == 15); // 10 + 5 bytes
    assert(metrics.errors == 0);
    
    SipMetrics sipMetrics = pjsip_get_session_metrics();
    assert(sipMetrics.packetsSent == 2);
    assert(sipMetrics.bytesSent == 15);
    
    std::cout << "End-to-end workflow test completed successfully." << std::endl;
}

void test_error_scenarios() {
    std::cout << "Testing error scenarios..." << std::endl;
    
    reset_metrics();
    
    // Test empty number call
    assert(pjsip_make_call("") == false);
    
    // Test invalid UDPTL send
    assert(pjsip_send_udptl(nullptr, 0) == false);
    
    // Test empty page encoding
    std::vector<uint8_t> emptyPage;
    std::vector<uint8_t> outFrames;
    assert(spandsp_encode_page(emptyPage, outFrames) == false);
    
    // Verify error count
    FoipMetrics metrics = get_foip_metrics();
    assert(metrics.errors == 3);
    
    std::cout << "Error scenarios test completed successfully." << std::endl;
}

int main() {
    test_end_to_end_workflow();
    test_error_scenarios();
    
    std::cout << "All integration tests passed!" << std::endl;
    return 0;
}