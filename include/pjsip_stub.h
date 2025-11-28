#ifndef PJSIP_STUB_H
#define PJSIP_STUB_H

#include <string>
#include <iostream>
#include <cstdint>
#include <cstddef>

bool pjsip_make_call(const std::string& number);
bool pjsip_send_udptl(const uint8_t* data, size_t len);
bool pjsip_hangup();

#endif
