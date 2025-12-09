#include <iostream>
#include <cassert>
#include "../include/logger.h"

void test_logging_functionality() {
    std::cout << "Testing logging functionality..." << std::endl;
    
    // Test different log levels
    log_debug("TEST", "Debug message test");
    log_info("TEST", "Info message test");
    log_warn("TEST", "Warning message test");
    log_error("TEST", "Error message test");
    
    std::cout << "Logging test completed." << std::endl;
}

void test_metrics_functionality() {
    std::cout << "Testing metrics functionality..." << std::endl;
    
    // Reset metrics
    reset_metrics();
    
    // Record some test data
    record_frame("TEST", 100);
    record_frame("TEST", 200);
    record_error("TEST");
    
    // Get metrics
    FoipMetrics metrics = get_foip_metrics();
    
    // Verify metrics
    assert(metrics.totalFrames == 2);
    assert(metrics.totalBytes == 300);
    assert(metrics.errors == 1);
    
    std::cout << "Metrics test completed successfully." << std::endl;
}

int main() {
    test_logging_functionality();
    test_metrics_functionality();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}