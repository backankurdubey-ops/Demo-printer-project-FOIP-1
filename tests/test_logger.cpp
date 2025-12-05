#include <iostream>
#include <cassert>
#include "../include/logger.h"

void test_logging_functions() {
    init_logger();
    
    log_debug("Test debug message");
    log_info("Test info message");
    log_warn("Test warning message");
    log_error("Test error message");
    
    std::cout << "Logging functions test passed" << std::endl;
}

void test_metrics_recording() {
    init_logger();
    
    // Record some test frames
    record_frame(100);
    record_frame(200);
    record_error();
    
    FoipMetrics metrics = get_foip_metrics();
    
    assert(metrics.totalFrames == 2);
    assert(metrics.totalBytes == 300);
    assert(metrics.errors == 1);
    
    std::cout << "Metrics recording test passed" << std::endl;
}

void test_session_timing() {
    init_logger();
    
    start_session();
    
    // Simulate some work
    record_frame(50);
    
    end_session();
    
    FoipMetrics metrics = get_foip_metrics();
    assert(metrics.sessionEndTimestamp > metrics.sessionStartTimestamp);
    
    std::cout << "Session timing test passed" << std::endl;
}

int main() {
    test_logging_functions();
    test_metrics_recording();
    test_session_timing();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
