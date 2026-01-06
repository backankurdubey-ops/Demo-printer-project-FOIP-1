#ifndef IFAX_SESSION_H
#define IFAX_SESSION_H

#include "FaxTypes.h"
#include "IFaxTransport.h"
#include "IFaxProtocol.h"

namespace foip {

class IFaxSession {
public:
    virtual ~IFaxSession() = default;
    
    // Session lifecycle
    virtual FaxResult initialize(const FaxSessionConfig& config) = 0;
    virtual FaxResult start() = 0;
    virtual FaxResult stop() = 0;
    virtual FaxResult cleanup() = 0;
    
    // Document operations
    virtual FaxResult sendFax(const FaxDocument& document, const std::string& destination) = 0;
    virtual FaxResult receiveFax(FaxDocument& document) = 0;
    
    // Status monitoring
    virtual FaxState getCurrentState() const = 0;
    virtual std::string getLastError() const = 0;
    
    // Component access
    virtual IFaxTransport* getTransport() = 0;
    virtual IFaxProtocol* getProtocol() = 0;
};

} // namespace foip

#endif
