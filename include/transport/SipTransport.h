#ifndef SIP_TRANSPORT_H
#define SIP_TRANSPORT_H

#include "abstract/IFaxTransport.h"

namespace foip {

class SipTransport : public IFaxTransport {
public:
    SipTransport();
    ~SipTransport() override;
    
    // IFaxTransport implementation
    FaxResult connect(const std::string& destination) override;
    FaxResult disconnect() override;
    bool isConnected() const override;
    
    FaxResult sendData(const uint8_t* data, size_t length) override;
    FaxResult receiveData(std::vector<uint8_t>& buffer) override;
    
    FaxResult configure(const FaxSessionConfig& config) override;
    TransportType getType() const override { return TransportType::SIP_UDPTL; }
    
    void setStateCallback(FaxStateCallback callback) override;
    
private:
    bool connected_;
    FaxStateCallback stateCallback_;
    FaxSessionConfig config_;
    
    // Internal methods
    FaxResult initializePjsip();
    FaxResult cleanupPjsip();
};

} // namespace foip

#endif
