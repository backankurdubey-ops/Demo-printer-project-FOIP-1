#include "abstract/FaxManager.h"
#include "transport/SipTransport.h"
#include "protocol/T30Protocol.h"
#include "utils/Logger.h"
#include <memory>

namespace foip {

class FaxSession : public IFaxSession {
public:
    FaxSession(std::unique_ptr<IFaxTransport> transport, 
               std::unique_ptr<IFaxProtocol> protocol)
        : transport_(std::move(transport))
        , protocol_(std::move(protocol))
        , currentState_(FaxState::IDLE) {}
    
    FaxResult initialize(const FaxSessionConfig& config) override {
        config_ = config;
        
        FaxResult result = transport_->configure(config);
        if (result != FaxResult::SUCCESS) {
            return result;
        }
        
        result = protocol_->initializeSession(config);
        if (result != FaxResult::SUCCESS) {
            return result;
        }
        
        currentState_ = FaxState::IDLE;
        return FaxResult::SUCCESS;
    }
    
    FaxResult start() override {
        currentState_ = FaxState::CONNECTING;
        return FaxResult::SUCCESS;
    }
    
    FaxResult stop() override {
        FaxResult transportResult = transport_->disconnect();
        FaxResult protocolResult = protocol_->terminateSession();
        
        currentState_ = FaxState::IDLE;
        
        return (transportResult == FaxResult::SUCCESS && protocolResult == FaxResult::SUCCESS) 
               ? FaxResult::SUCCESS : FaxResult::FAILURE;
    }
    
    FaxResult cleanup() override {
        return stop();
    }
    
    FaxResult sendFax(const FaxDocument& document, const std::string& destination) override {
        Logger::getInstance().log("FaxSession sending fax to: " + destination);
        
        // Connect transport
        FaxResult result = transport_->connect(destination);
        if (result != FaxResult::SUCCESS) {
            return result;
        }
        
        // Negotiate capabilities
        result = protocol_->negotiateCapabilities();
        if (result != FaxResult::SUCCESS) {
            transport_->disconnect();
            return result;
        }
        
        // Send document
        result = protocol_->sendDocument(document);
        if (result != FaxResult::SUCCESS) {
            transport_->disconnect();
            return result;
        }
        
        // Cleanup
        transport_->disconnect();
        return FaxResult::SUCCESS;
    }
    
    FaxResult receiveFax(FaxDocument& document) override {
        Logger::getInstance().log("FaxSession receiving fax");
        
        return protocol_->receiveDocument(document);
    }
    
    FaxState getCurrentState() const override {
        return currentState_;
    }
    
    std::string getLastError() const override {
        return lastError_;
    }
    
    IFaxTransport* getTransport() override {
        return transport_.get();
    }
    
    IFaxProtocol* getProtocol() override {
        return protocol_.get();
    }
    
private:
    std::unique_ptr<IFaxTransport> transport_;
    std::unique_ptr<IFaxProtocol> protocol_;
    FaxSessionConfig config_;
    FaxState currentState_;
    std::string lastError_;
};

FaxManager& FaxManager::getInstance() {
    static FaxManager instance;
    return instance;
}

std::unique_ptr<IFaxSession> FaxManager::createSession(const FaxSessionConfig& config) {
    Logger::getInstance().log("FaxManager creating session");
    
    // Create transport based on configuration
    std::unique_ptr<IFaxTransport> transport;
    switch (config.transport) {
        case TransportType::SIP_UDPTL:
            transport = std::make_unique<SipTransport>();
            break;
        default:
            Logger::getInstance().log("Unsupported transport type");
            return nullptr;
    }
    
    // Create protocol based on configuration
    std::unique_ptr<IFaxProtocol> protocol;
    switch (config.protocol) {
        case ProtocolType::T30_OVER_G711:
        case ProtocolType::T38_REALTIME:
            protocol = std::make_unique<T30Protocol>();
            break;
        default:
            Logger::getInstance().log("Unsupported protocol type");
            return nullptr;
    }
    
    auto session = std::make_unique<FaxSession>(std::move(transport), std::move(protocol));
    
    FaxResult result = session->initialize(config);
    if (result != FaxResult::SUCCESS) {
        Logger::getInstance().log("Failed to initialize session");
        return nullptr;
    }
    
    return session;
}

FaxResult FaxManager::sendFax(const std::string& destination, const FaxDocument& document) {
    Logger::getInstance().log("FaxManager sending fax to: " + destination);
    
    auto session = createSession(defaultConfig_);
    if (!session) {
        return FaxResult::FAILURE;
    }
    
    return session->sendFax(document, destination);
}

FaxResult FaxManager::receiveFax(FaxDocument& document) {
    Logger::getInstance().log("FaxManager receiving fax");
    
    auto session = createSession(defaultConfig_);
    if (!session) {
        return FaxResult::FAILURE;
    }
    
    return session->receiveFax(document);
}

void FaxManager::setDefaultConfig(const FaxSessionConfig& config) {
    defaultConfig_ = config;
}

FaxSessionConfig FaxManager::getDefaultConfig() const {
    return defaultConfig_;
}

void FaxManager::registerSession(const std::string& sessionId, 
                                std::shared_ptr<IFaxSession> session) {
    activeSessions_[sessionId] = session;
}

void FaxManager::unregisterSession(const std::string& sessionId) {
    activeSessions_.erase(sessionId);
}

std::shared_ptr<IFaxSession> FaxManager::getSession(const std::string& sessionId) {
    auto it = activeSessions_.find(sessionId);
    return (it != activeSessions_.end()) ? it->second : nullptr;
}

} // namespace foip
