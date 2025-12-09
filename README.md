# FOIP Direct Demo Project v1.1

## Overview

This project demonstrates a simplified Fax Over IP (FOIP) transmission flow using stubbed PJSIP (SIP signaling, UDPTL transport) and stubbed SpanDSP (T.30 fax protocol). Version 1.1 introduces a robust logging and metrics subsystem for enhanced observability and debugging.

## Features

- **SIP Signaling Layer**: Stubbed PJSIP implementation with metrics tracking
- **Fax Protocol Layer**: Stubbed SpanDSP implementation with frame-level metrics
- **Logging & Metrics Subsystem**: Centralized logging with multiple levels and comprehensive metrics collection
- **Error Handling**: Structured error logging and metrics tracking
- **Testing Framework**: Unit tests and integration tests for validation

## Directory Structure

```
foip_direct_demo/
├── CMakeLists.txt
├── Makefile
├── README.md
├── include/
│   ├── logger.h
│   ├── pjsip_stub.h
│   └── spandsp_stub.h
├── src/
│   ├── main.cpp
│   ├── logger.cpp
│   ├── pjsip_stub.cpp
│   └── spandsp_stub.cpp
├── tests/
│   ├── test_logger.cpp
│   └── test_integration.cpp
├── build/
│   └── (object files)
└── libs/
    └── (libraries)
```

## Building

### Using CMake
```bash
mkdir build && cd build
cmake ..
make
```

### Using Makefile
```bash
make
```

## Running

```bash
./foip_direct_demo
```

## Testing

```bash
make test
```

## Sample Output

```
[12:34:56.789] [INFO] [APP] === DIRECT FOIP STACK CALL DEMO ===
[12:34:56.790] [INFO] [APP] Step 1: Establishing SIP call
[12:34:56.791] [INFO] [PJSIP] Making SIP call to 1234567890
[12:34:56.792] [INFO] [PJSIP] SIP call established successfully
[12:34:56.793] [INFO] [APP] Step 2: Starting fax session
[12:34:56.794] [INFO] [SpanDSP] Initializing T.30 fax session
...

=== FOIP SESSION METRICS ===
Frames Sent: 2
Total Bytes: 15
Errors: 0
Session Duration: 45 ms
SIP Packets Sent: 2
SIP Bytes Sent: 15
=== END ===
```

## Architecture

The system consists of four main components:

1. **SIP Signaling Layer (PJSIP Stub)**: Handles SIP call setup, UDPTL transmission, and call teardown
2. **Fax Protocol Layer (SpanDSP Stub)**: Manages T.30 fax sessions and page encoding
3. **Logging & Metrics Subsystem**: Provides centralized logging and metrics collection
4. **Main Application**: Orchestrates the complete FOIP workflow

## API Reference

### Logging API
- `log_debug(module, message)`: Debug level logging
- `log_info(module, message)`: Info level logging  
- `log_warn(module, message)`: Warning level logging
- `log_error(module, message)`: Error level logging

### Metrics API
- `record_frame(module, frameSize)`: Record frame transmission
- `record_error(module)`: Record error occurrence
- `get_foip_metrics()`: Get aggregated session metrics

## License

MIT License