#include "spandsp_stub.h"
#include "logger.h"

// Global SpanDSP metrics
static size_t g_frames_generated = 0;
static size_t g_total_encoded_bytes = 0;
static size_t g_encoding_errors = 0;

bool spandsp_start_fax_session() {
    log_info("SpanDSP", "Initializing T.30 fax session");
    
    // Reset SpanDSP metrics for new session
    g_frames_generated = 0;
    g_total_encoded_bytes = 0;
    g_encoding_errors = 0;
    
    log_info("SpanDSP", "T.30 fax session initialized successfully");
    return true;
}

bool spandsp_encode_page(const std::vector<uint8_t>& page, 
                         std::vector<uint8_t>& outFrames) {
    
    if (page.empty()) {
        log_error("SpanDSP", "Page encoding failed: empty page data");
        g_encoding_errors++;
        record_error("SpanDSP");
        return false;
    }
    
    log_info("SpanDSP", "Encoding fax page (" + std::to_string(page.size()) + " bytes)");

    // Simulate encoding process - create fake encoded data
    outFrames.assign(10, 0xAA); // fake encoded data
    
    // Update SpanDSP metrics
    g_frames_generated++;
    g_total_encoded_bytes += outFrames.size();
    
    // Record frame metrics
    spandsp_record_frame(outFrames.size());
    
    log_info("SpanDSP", "Page encoded successfully (" + std::to_string(outFrames.size()) + " bytes)");
    return true;
}

bool spandsp_end_fax_session(std::vector<uint8_t>& finalFrames) {
    log_info("SpanDSP", "Finalizing fax session");

    // Generate termination frames
    finalFrames.assign(5, 0xFF); // fake termination frames
    
    // Update metrics
    g_frames_generated++;
    g_total_encoded_bytes += finalFrames.size();
    
    // Record final frame
    spandsp_record_frame(finalFrames.size());
    
    log_info("SpanDSP", "Fax session finalized with " + std::to_string(g_frames_generated) + 
             " frames, " + std::to_string(g_total_encoded_bytes) + " total bytes");
    
    if (g_encoding_errors > 0) {
        log_warn("SpanDSP", "Session completed with " + std::to_string(g_encoding_errors) + " encoding errors");
    }
    
    return true;
}

void spandsp_record_frame(size_t frameSize) {
    record_frame("SpanDSP", frameSize);
    log_debug("SpanDSP", "Recorded frame: " + std::to_string(frameSize) + " bytes");
}