#ifndef PJSIP_STUB_H
#define PJSIP_STUB_H

#include <string>
#include <iostream>
#include <cstdint>

// SIP metrics structure
struct SipMetrics {
    size_t packetsSent;
    size_t bytesSent;
    uint64_t sessionDurationMs;
};

// Original PJSIP API
bool pjsip_make_call(const std::string& number);
bool pjsip_send_udptl(const uint8_t* data, size_t len);
bool pjsip_hangup();

// New metrics API
SipMetrics pjsip_get_session_metrics();
void pjsip_reset_metrics();

#endif