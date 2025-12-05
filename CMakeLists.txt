cmake_minimum_required(VERSION 3.10)

project(foip_direct_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(foip_direct_demo
    src/main.cpp
    src/pjsip_stub.cpp
    src/spandsp_stub.cpp
    src/logger.cpp
)

# Create build directory for object files
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

# Enable testing
enable_testing()

# Add unit tests
add_executable(test_logger
    tests/test_logger.cpp
    src/logger.cpp
)

add_test(NAME LoggerTest COMMAND test_logger)
