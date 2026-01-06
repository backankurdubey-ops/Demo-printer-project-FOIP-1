#ifndef IFAX_TRANSPORT_H
#define IFAX_TRANSPORT_H

#include "FaxTypes.h"

namespace foip {

class IFaxTransport {
public:
    virtual ~IFaxTransport() = default;
    
    // Connection management
    virtual FaxResult connect(const std::string& destination) = 0;
    virtual FaxResult disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // Data transmission
    virtual FaxResult sendData(const uint8_t* data, size_t length) = 0;
    virtual FaxResult receiveData(std::vector<uint8_t>& buffer) = 0;
    
    // Transport configuration
    virtual FaxResult configure(const FaxSessionConfig& config) = 0;
    virtual TransportType getType() const = 0;
    
    // Event callbacks
    virtual void setStateCallback(FaxStateCallback callback) = 0;
};

} // namespace foip

#endif
