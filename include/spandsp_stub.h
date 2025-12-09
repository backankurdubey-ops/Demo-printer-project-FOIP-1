#ifndef SPANDSP_STUB_H
#define SPANDSP_STUB_H

#include <vector>
#include <iostream>

// Original SpanDSP API
bool spandsp_start_fax_session();
bool spandsp_encode_page(const std::vector<uint8_t>& page, std::vector<uint8_t>& outFrames);
bool spandsp_end_fax_session(std::vector<uint8_t>& finalFrames);

// New metrics hook
void spandsp_record_frame(size_t frameSize);

#endif