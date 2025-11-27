# FOIP Direct Demo Project - Technical Specification

## Project Overview

**Project Name:** foip_direct_demo  
**Version:** 1.0  
**License:** MIT License  
**Language:** C++17  
**Build System:** CMake 3.10+  

### Purpose
This project demonstrates a simplified Fax Over IP (FOIP) implementation using stub implementations of PJSIP (SIP signaling) and SpanDSP (T.30 fax protocol) libraries. It showcases the basic workflow of establishing a SIP call, encoding fax data, transmitting it via UDPTL, and terminating the session.

## Architecture Overview

### System Components

1. **SIP Signaling Layer (PJSIP Stub)**
   - Handles SIP call establishment
   - Manages UDPTL packet transmission
   - Provides call termination functionality

2. **Fax Protocol Layer (SpanDSP Stub)**
   - Implements T.30 fax session management
   - Handles fax page encoding
   - Manages session initialization and termination

3. **Main Application**
   - Orchestrates the complete FOIP workflow
   - Coordinates between SIP and fax protocol layers

### Protocol Flow

```
SIP INVITE → T.30 Session Start → Page Encoding → UDPTL Transmission → Session End → SIP BYE
```

## Directory Structure

```
foip_direct_demo/
├── CMakeLists.txt          # Build configuration
├── LICENSE                 # MIT License
├── .gitignore             # Git ignore rules
├── include/               # Header files
│   ├── pjsip_stub.h       # PJSIP interface declarations
│   └── spandsp_stub.h     # SpanDSP interface declarations
└── src/                   # Source files
    ├── main.cpp           # Main application logic
    ├── pjsip_stub.cpp     # PJSIP stub implementation
    └── spandsp_stub.cpp   # SpanDSP stub implementation
```

## Module Specifications

### 1. PJSIP Stub Module

**File:** `include/pjsip_stub.h`, `src/pjsip_stub.cpp`

**Purpose:** Provides SIP signaling and UDPTL transport functionality

**API Functions:**

```cpp
bool pjsip_make_call(const std::string& number);
```
- **Description:** Initiates a SIP call to the specified number
- **Parameters:** `number` - Target phone number as string
- **Returns:** `true` on success, `false` on failure
- **Protocol:** Simulates SIP INVITE message

```cpp
bool pjsip_send_udptl(const uint8_t* data, size_t len);
```
- **Description:** Transmits fax data via UDPTL protocol
- **Parameters:** 
  - `data` - Pointer to encoded fax data
  - `len` - Size of data in bytes
- **Returns:** `true` on successful transmission
- **Protocol:** Encapsulates T.38 data in UDPTL packets

```cpp
bool pjsip_hangup();
```
- **Description:** Terminates the active SIP call
- **Returns:** `true` on successful hangup
- **Protocol:** Sends SIP BYE message

### 2. SpanDSP Stub Module

**File:** `include/spandsp_stub.h`, `src/spandsp_stub.cpp`

**Purpose:** Handles T.30 fax protocol operations and page encoding

**API Functions:**

```cpp
bool spandsp_start_fax_session();
```
- **Description:** Initializes a T.30 fax transmission session
- **Returns:** `true` on successful initialization
- **Protocol:** Sets up T.30 state machine

```cpp
bool spandsp_encode_page(const std::vector<uint8_t>& page, std::vector<uint8_t>& outFrames);
```
- **Description:** Encodes a fax page into T.30 protocol frames
- **Parameters:**
  - `page` - Input page data (raw bytes)
  - `outFrames` - Output encoded frames
- **Returns:** `true` on successful encoding
- **Protocol:** Applies T.30 encoding (MH, MR, or MMR compression)

```cpp
bool spandsp_end_fax_session(std::vector<uint8_t>& finalFrames);
```
- **Description:** Finalizes fax session and generates termination frames
- **Parameters:** `finalFrames` - Output termination frames
- **Returns:** `true` on successful session termination
- **Protocol:** Generates T.30 end-of-page and disconnect frames

### 3. Main Application

**File:** `src/main.cpp`

**Purpose:** Orchestrates the complete FOIP workflow

**Workflow Steps:**
1. **Call Establishment:** Uses PJSIP to establish SIP call
2. **Session Initialization:** Starts T.30 fax session via SpanDSP
3. **Page Processing:** Encodes sample page data
4. **Data Transmission:** Sends encoded frames via UDPTL
5. **Session Termination:** Ends fax session and generates final frames
6. **Final Transmission:** Sends termination frames
7. **Call Termination:** Hangs up SIP call

## Protocol Implementation Details

### SIP/SDP Integration
- **Call Setup:** Simulates SIP INVITE with T.38 capability negotiation
- **Media Negotiation:** Assumes successful T.38 parameter agreement
- **Transport:** Uses UDPTL as transport protocol for T.38 packets

### T.30/T.38 Protocol Handling
- **T.30 State Machine:** Simplified implementation focusing on sender role
- **T.38 Packetization:** Encapsulates T.30 frames in T.38 IFP packets
- **Error Handling:** Basic success/failure reporting

### Data Flow

```
Raw Page Data → T.30 Encoding → T.38 Packetization → UDPTL Transport → SIP Transmission
```

## Build Configuration

**CMake Requirements:**
- Minimum CMake version: 3.10
- C++ Standard: C++17
- Include directories: `include/`

**Build Targets:**
- **Executable:** `foip_direct_demo`
- **Source Files:** 
  - `src/main.cpp`
  - `src/pjsip_stub.cpp`
  - `src/spandsp_stub.cpp`

**Build Commands:**
```bash
mkdir build
cd build
cmake ..
make
```

## Dependencies

### External Libraries (Stubbed)
- **PJSIP:** SIP protocol stack (stubbed implementation)
- **SpanDSP:** DSP and fax protocol library (stubbed implementation)

### Standard Libraries
- `<iostream>` - Console I/O operations
- `<vector>` - Dynamic array container
- `<string>` - String manipulation

## Error Handling

### Error Scenarios
1. **SIP Call Failure:** Returns error code 1 if call establishment fails
2. **Protocol Errors:** Boolean return values indicate success/failure
3. **Resource Management:** Automatic cleanup via RAII principles

### Logging
- **Console Output:** All operations logged to stdout with module prefixes
- **Format:** `[MODULE] Operation description`
- **Examples:**
  - `[PJSIP] Making SIP call to 1234567890`
  - `[SpanDSP] Encoding fax page (5 bytes)`

## Testing and Validation

### Test Scenarios
1. **Complete Workflow:** End-to-end fax transmission simulation
2. **Module Integration:** Verify proper interaction between PJSIP and SpanDSP stubs
3. **Error Conditions:** Test failure handling and cleanup

### Expected Output
```
=== DIRECT FOIP STACK CALL DEMO ===
[PJSIP] Making SIP call to 1234567890
[SpanDSP] Initializing T.30 fax session
[SpanDSP] Encoding fax page (5 bytes)
[PJSIP] Sending UDPTL packet (size=10)
[SpanDSP] Finalizing fax session
[PJSIP] Sending UDPTL packet (size=5)
[PJSIP] Hanging up SIP call
=== FAX SEND COMPLETE ===
```

## Future Enhancements

### Potential Improvements
1. **Real Library Integration:** Replace stubs with actual PJSIP and SpanDSP libraries
2. **Receive Functionality:** Add fax reception capabilities
3. **Multi-page Support:** Handle multiple page documents
4. **Error Recovery:** Implement robust error handling and retry mechanisms
5. **Configuration Management:** Add configuration file support
6. **Logging Framework:** Implement structured logging with different levels

### Scalability Considerations
1. **Concurrent Sessions:** Support multiple simultaneous fax sessions
2. **Protocol Extensions:** Add support for additional T.38 features
3. **Performance Optimization:** Optimize encoding and transmission performance

## Security Considerations

### Current Implementation
- **No Authentication:** Stub implementation lacks security features
- **No Encryption:** Data transmitted in plain text

### Production Requirements
- **SIP Authentication:** Implement digest authentication
- **TLS/SRTP:** Secure transport layer encryption
- **Access Control:** Implement proper authorization mechanisms

## Compliance and Standards

### Supported Standards
- **ITU-T T.30:** Fax protocol procedures (simulated)
- **ITU-T T.38:** Real-time fax over IP (simulated)
- **RFC 3261:** SIP protocol (simulated)
- **RFC 3362:** T.38 over SIP (simulated)

## Conclusion

This FOIP Direct Demo project provides a foundational understanding of how Fax Over IP systems integrate SIP signaling with T.30/T.38 fax protocols. While using stub implementations, it demonstrates the essential workflow and component interactions required for a production FOIP system.

The modular design allows for easy replacement of stub components with real library implementations, making it an excellent starting point for developing production-grade FOIP solutions.
