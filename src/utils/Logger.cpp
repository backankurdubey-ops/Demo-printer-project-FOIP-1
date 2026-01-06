#include "utils/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

namespace foip {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::string logMessage = "[" + std::to_string(time_t) + "] " + message;
    
    std::cout << logMessage << std::endl;
    
    if (logFile_.is_open()) {
        logFile_ << logMessage << std::endl;
        logFile_.flush();
    }
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (logFile_.is_open()) {
        logFile_.close();
    }
    
    logFile_.open(filename, std::ios::app);
}

void Logger::setLogLevel(int level) {
    logLevel_ = level;
}

} // namespace foip
