#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

namespace foip {

class Logger {
public:
    static Logger& getInstance();
    
    void log(const std::string& message);
    void setLogFile(const std::string& filename);
    void setLogLevel(int level);
    
private:
    Logger() = default;
    std::mutex mutex_;
    std::ofstream logFile_;
    int logLevel_ = 0;
};

} // namespace foip

#endif
