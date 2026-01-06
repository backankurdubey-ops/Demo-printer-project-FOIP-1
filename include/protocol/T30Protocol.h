#ifndef T30_PROTOCOL_H
#define T30_PROTOCOL_H

#include "abstract/IFaxProtocol.h"

namespace foip {

class T30Protocol : public IFaxProtocol {
public:
    T30Protocol();
    ~T30Protocol() override;
    
    // IFaxProtocol implementation
    FaxResult initializeSession(const FaxSessionConfig& config) override;
    FaxResult terminateSession() override;
    
    FaxResult sendDocument(const FaxDocument& document) override;
    FaxResult receiveDocument(FaxDocument& document) override;
    
    FaxResult negotiateCapabilities() override;
    FaxResult handleProtocolFrames(const uint8_t* data, size_t length) override;
    
    FaxState getState() const override { return currentState_; }
    ProtocolType getType() const override { return ProtocolType::T30_OVER_G711; }
    
    void setProgressCallback(FaxProgressCallback callback) override;
    
private:
    FaxState currentState_;
    FaxSessionConfig config_;
    FaxProgressCallback progressCallback_;
    
    // Internal methods
    FaxResult initializeSpanDsp();
    FaxResult cleanupSpanDsp();
    FaxResult encodeDocumentPage(const std::vector<uint8_t>& pageData, 
                                std::vector<uint8_t>& encodedData);
};

} // namespace foip

#endif
