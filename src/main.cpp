#include <iostream>
#include "abstract/FaxManager.h"
#include "utils/Logger.h"

using namespace foip;

int main() {
    std::cout << "=== FOIP ABSTRACT LAYER DEMO ===\n";
    
    // Initialize logger
    Logger::getInstance().log("Application starting");
    
    // Configure default fax session
    FaxSessionConfig config;
    config.localId = "LocalFaxMachine";
    config.remoteNumber = "1234567890";
    config.transport = TransportType::SIP_UDPTL;
    config.protocol = ProtocolType::T30_OVER_G711;
    config.timeout_ms = 30000;
    config.ecm_enabled = true;
    config.max_bitrate = 14400;
    
    FaxManager& manager = FaxManager::getInstance();
    manager.setDefaultConfig(config);
    
    // Create a sample document
    FaxDocument document;
    document.data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    document.format = "TIFF";
    document.pages = 1;
    
    // Send fax using high-level API
    FaxResult result = manager.sendFax(config.remoteNumber, document);
    
    if (result == FaxResult::SUCCESS) {
        std::cout << "Fax sent successfully!\n";
    } else {
        std::cerr << "Failed to send fax\n";
        return 1;
    }
    
    // Alternative: Use session-based API for more control
    std::cout << "\n=== SESSION-BASED API DEMO ===\n";
    
    auto session = manager.createSession(config);
    if (!session) {
        std::cerr << "Failed to create fax session\n";
        return 1;
    }
    
    // Set up callbacks
    session->getTransport()->setStateCallback([](FaxState state, const std::string& message) {
        std::cout << "Transport State: " << static_cast<int>(state) << " - " << message << "\n";
    });
    
    session->getProtocol()->setProgressCallback([](size_t current, size_t total) {
        std::cout << "Progress: " << current << "/" << total << " pages\n";
    });
    
    // Send fax with session
    result = session->sendFax(document, config.remoteNumber);
    
    if (result == FaxResult::SUCCESS) {
        std::cout << "Session-based fax sent successfully!\n";
    } else {
        std::cerr << "Failed to send fax via session\n";
    }
    
    Logger::getInstance().log("Application completed");
    std::cout << "=== DEMO COMPLETE ===\n";
    
    return 0;
}
