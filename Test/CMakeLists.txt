
cmake_minimum_required(VERSION 3.10)

project(foip_tests_only LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
include(CTest)
enable_testing()

# Find GTest and GMock
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# Test executable (only tests)
add_executable(foip_tests
    tests/test_main.cpp
    tests/test_pjsip_stub.cpp
    tests/test_spandsp_stub.cpp
    tests/test_foip_integration.cpp
)

target_include_directories(foip_tests PRIVATE include tests)

target_link_libraries(foip_tests
    GTest::GTest
    GTest::Main
    GMock::GMock
    GMock::Main
    pthread
)

# Register tests with CTest
add_test(NAME AllTests COMMAND foip_tests)
