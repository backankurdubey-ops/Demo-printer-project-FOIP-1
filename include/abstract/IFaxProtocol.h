#ifndef IFAX_PROTOCOL_H
#define IFAX_PROTOCOL_H

#include "FaxTypes.h"

namespace foip {

class IFaxProtocol {
public:
    virtual ~IFaxProtocol() = default;
    
    // Session management
    virtual FaxResult initializeSession(const FaxSessionConfig& config) = 0;
    virtual FaxResult terminateSession() = 0;
    
    // Document handling
    virtual FaxResult sendDocument(const FaxDocument& document) = 0;
    virtual FaxResult receiveDocument(FaxDocument& document) = 0;
    
    // Protocol negotiation
    virtual FaxResult negotiateCapabilities() = 0;
    virtual FaxResult handleProtocolFrames(const uint8_t* data, size_t length) = 0;
    
    // Status and monitoring
    virtual FaxState getState() const = 0;
    virtual ProtocolType getType() const = 0;
    
    // Event callbacks
    virtual void setProgressCallback(FaxProgressCallback callback) = 0;
};

} // namespace foip

#endif
