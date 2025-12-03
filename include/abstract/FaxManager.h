#ifndef FAX_MANAGER_H
#define FAX_MANAGER_H

#include "IFaxSession.h"
#include <memory>
#include <map>

namespace foip {

class FaxManager {
public:
    static FaxManager& getInstance();
    
    // Factory methods
    std::unique_ptr<IFaxSession> createSession(const FaxSessionConfig& config);
    
    // High-level operations
    FaxResult sendFax(const std::string& destination, const FaxDocument& document);
    FaxResult receiveFax(FaxDocument& document);
    
    // Configuration
    void setDefaultConfig(const FaxSessionConfig& config);
    FaxSessionConfig getDefaultConfig() const;
    
    // Session management
    void registerSession(const std::string& sessionId, std::shared_ptr<IFaxSession> session);
    void unregisterSession(const std::string& sessionId);
    std::shared_ptr<IFaxSession> getSession(const std::string& sessionId);
    
private:
    FaxManager() = default;
    FaxSessionConfig defaultConfig_;
    std::map<std::string, std::shared_ptr<IFaxSession>> activeSessions_;
};

} // namespace foip

#endif
