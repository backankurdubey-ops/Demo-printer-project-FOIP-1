#ifndef PJSIP_STUB_H
#define PJSIP_STUB_H

#include <string>
#include <iostream>
#include "logger.h"

bool pjsip_make_call(const std::string& number);
bool pjsip_send_udptl(const uint8_t* data, size_t len);
bool pjsip_hangup();
SipMetrics pjsip_get_session_metrics();

#endif
