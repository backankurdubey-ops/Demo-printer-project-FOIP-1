#include "protocol/T30Protocol.h"
#include "legacy/spandsp_stub.h"
#include "utils/Logger.h"

namespace foip {

T30Protocol::T30Protocol() : currentState_(FaxState::IDLE) {
    Logger::getInstance().log("T30Protocol created");
}

T30Protocol::~T30Protocol() {
    if (currentState_ != FaxState::IDLE) {
        terminateSession();
    }
}

FaxResult T30Protocol::initializeSession(const FaxSessionConfig& config) {
    Logger::getInstance().log("T30Protocol initializing session");
    
    config_ = config;
    currentState_ = FaxState::CONNECTING;
    
    // Use legacy SpanDSP stub
    if (!spandsp_start_fax_session()) {
        Logger::getInstance().log("Failed to initialize SpanDSP session");
        currentState_ = FaxState::ERROR;
        return FaxResult::PROTOCOL_ERROR;
    }
    
    currentState_ = FaxState::NEGOTIATING;
    return FaxResult::SUCCESS;
}

FaxResult T30Protocol::terminateSession() {
    Logger::getInstance().log("T30Protocol terminating session");
    
    std::vector<uint8_t> finalFrames;
    
    // Use legacy SpanDSP stub
    if (!spandsp_end_fax_session(finalFrames)) {
        Logger::getInstance().log("Failed to terminate SpanDSP session");
        return FaxResult::PROTOCOL_ERROR;
    }
    
    currentState_ = FaxState::IDLE;
    return FaxResult::SUCCESS;
}

FaxResult T30Protocol::sendDocument(const FaxDocument& document) {
    if (currentState_ != FaxState::NEGOTIATING && currentState_ != FaxState::TRANSMITTING) {
        return FaxResult::INVALID_PARAMETER;
    }
    
    Logger::getInstance().log("T30Protocol sending document with " + 
                             std::to_string(document.pages) + " pages");
    
    currentState_ = FaxState::TRANSMITTING;
    
    // Process each page
    for (size_t page = 0; page < document.pages; ++page) {
        std::vector<uint8_t> encodedData;
        
        // For demo, use the entire document data as a single page
        FaxResult result = encodeDocumentPage(document.data, encodedData);
        if (result != FaxResult::SUCCESS) {
            currentState_ = FaxState::ERROR;
            return result;
        }
        
        if (progressCallback_) {
            progressCallback_(page + 1, document.pages);
        }
    }
    
    currentState_ = FaxState::COMPLETING;
    return FaxResult::SUCCESS;
}

FaxResult T30Protocol::receiveDocument(FaxDocument& document) {
    Logger::getInstance().log("T30Protocol receiving document");
    
    currentState_ = FaxState::RECEIVING;
    
    // Implementation would handle incoming fax data
    // For now, create a dummy document
    document.data = {0x01, 0x02, 0x03, 0x04, 0x05};
    document.format = "TIFF";
    document.pages = 1;
    
    currentState_ = FaxState::COMPLETING;
    return FaxResult::SUCCESS;
}

FaxResult T30Protocol::negotiateCapabilities() {
    Logger::getInstance().log("T30Protocol negotiating capabilities");
    
    // Implementation would handle T.30 capability negotiation
    // For now, assume successful negotiation
    
    return FaxResult::SUCCESS;
}

FaxResult T30Protocol::handleProtocolFrames(const uint8_t* data, size_t length) {
    Logger::getInstance().log("T30Protocol handling " + std::to_string(length) + " bytes");
    
    // Implementation would process incoming T.30 frames
    // For now, just log the data
    
    return FaxResult::SUCCESS;
}

void T30Protocol::setProgressCallback(FaxProgressCallback callback) {
    progressCallback_ = callback;
}

FaxResult T30Protocol::encodeDocumentPage(const std::vector<uint8_t>& pageData, 
                                         std::vector<uint8_t>& encodedData) {
    Logger::getInstance().log("T30Protocol encoding page (" + 
                             std::to_string(pageData.size()) + " bytes)");
    
    // Use legacy SpanDSP stub
    if (!spandsp_encode_page(pageData, encodedData)) {
        Logger::getInstance().log("Failed to encode page data");
        return FaxResult::PROTOCOL_ERROR;
    }
    
    return FaxResult::SUCCESS;
}

} // namespace foip
