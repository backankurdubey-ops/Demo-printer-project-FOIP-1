#include "transport/SipTransport.h"
#include "legacy/pjsip_stub.h"
#include "utils/Logger.h"

namespace foip {

SipTransport::SipTransport() : connected_(false) {
    Logger::getInstance().log("SipTransport created");
}

SipTransport::~SipTransport() {
    if (connected_) {
        disconnect();
    }
}

FaxResult SipTransport::connect(const std::string& destination) {
    Logger::getInstance().log("SipTransport connecting to: " + destination);
    
    if (connected_) {
        return FaxResult::SUCCESS;
    }
    
    // Use legacy PJSIP stub
    if (!pjsip_make_call(destination)) {
        Logger::getInstance().log("Failed to establish SIP connection");
        return FaxResult::TRANSPORT_ERROR;
    }
    
    connected_ = true;
    
    if (stateCallback_) {
        stateCallback_(FaxState::CONNECTING, "SIP connection established");
    }
    
    return FaxResult::SUCCESS;
}

FaxResult SipTransport::disconnect() {
    if (!connected_) {
        return FaxResult::SUCCESS;
    }
    
    Logger::getInstance().log("SipTransport disconnecting");
    
    // Use legacy PJSIP stub
    if (!pjsip_hangup()) {
        Logger::getInstance().log("Failed to hangup SIP call");
        return FaxResult::TRANSPORT_ERROR;
    }
    
    connected_ = false;
    
    if (stateCallback_) {
        stateCallback_(FaxState::DISCONNECTED, "SIP connection closed");
    }
    
    return FaxResult::SUCCESS;
}

bool SipTransport::isConnected() const {
    return connected_;
}

FaxResult SipTransport::sendData(const uint8_t* data, size_t length) {
    if (!connected_) {
        return FaxResult::TRANSPORT_ERROR;
    }
    
    Logger::getInstance().log("SipTransport sending " + std::to_string(length) + " bytes");
    
    // Use legacy PJSIP stub for UDPTL transmission
    if (!pjsip_send_udptl(data, length)) {
        Logger::getInstance().log("Failed to send UDPTL data");
        return FaxResult::TRANSPORT_ERROR;
    }
    
    return FaxResult::SUCCESS;
}

FaxResult SipTransport::receiveData(std::vector<uint8_t>& buffer) {
    // Implementation would handle incoming UDPTL data
    // For now, return empty buffer
    buffer.clear();
    return FaxResult::SUCCESS;
}

FaxResult SipTransport::configure(const FaxSessionConfig& config) {
    config_ = config;
    Logger::getInstance().log("SipTransport configured for: " + config.remoteNumber);
    return FaxResult::SUCCESS;
}

void SipTransport::setStateCallback(FaxStateCallback callback) {
    stateCallback_ = callback;
}

} // namespace foip
