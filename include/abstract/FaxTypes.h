#ifndef FAX_TYPES_H
#define FAX_TYPES_H

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace foip {

enum class FaxResult {
    SUCCESS,
    FAILURE,
    TIMEOUT,
    PROTOCOL_ERROR,
    TRANSPORT_ERROR,
    INVALID_PARAMETER
};

enum class FaxState {
    IDLE,
    CONNECTING,
    NEGOTIATING,
    TRANSMITTING,
    RECEIVING,
    COMPLETING,
    DISCONNECTED,
    ERROR
};

enum class TransportType {
    SIP_UDPTL,
    T38_OVER_UDP,
    T38_OVER_TCP
};

enum class ProtocolType {
    T30_OVER_G711,
    T38_REALTIME,
    T38_STORE_FORWARD
};

struct FaxDocument {
    std::vector<uint8_t> data;
    std::string format;  // "TIFF", "PDF", "PCX"
    size_t pages;
};

struct FaxSessionConfig {
    std::string localId;
    std::string remoteNumber;
    TransportType transport;
    ProtocolType protocol;
    uint32_t timeout_ms;
    bool ecm_enabled;
    uint32_t max_bitrate;
};

using FaxStateCallback = std::function<void(FaxState, const std::string&)>;
using FaxProgressCallback = std::function<void(size_t current_page, size_t total_pages)>;

} // namespace foip

#endif
