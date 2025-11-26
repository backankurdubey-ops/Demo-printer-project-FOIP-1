#include "pjsip_stub.h"

bool pjsip_make_call(const std::string& number) {
    std::cout << "[PJSIP] Making SIP call to " << number << std::endl;
    return true;
}

bool pjsip_send_udptl(const uint8_t* data, size_t len) {
    std::cout << "[PJSIP] Sending UDPTL packet (size=" << len << ")" << std::endl;
    return true;
}

bool pjsip_hangup() {
    std::cout << "[PJSIP] Hanging up SIP call" << std::endl;
    return true;
}
