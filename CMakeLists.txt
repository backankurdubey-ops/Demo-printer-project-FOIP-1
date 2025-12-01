cmake_minimum_required(VERSION 3.10)
project(foip_direct_demo VERSION 1.0 LANGUAGES CXX)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include headers
include_directories(include)

# Source files
set(SOURCES
    src/pjsip_stub.cpp
    src/spandsp_stub.cpp
)

# ---------------------------------------------------------
# Build shared library (.so on Linux, .dll on Windows)
# ---------------------------------------------------------
add_library(foip_direct_demo SHARED ${SOURCES})
set_target_properties(foip_direct_demo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# ---------------------------------------------------------
# Build static library (.a on Linux, .lib on Windows)
# ---------------------------------------------------------
add_library(foip_direct_demo_static STATIC ${SOURCES})

# Give static lib same name as shared lib (but different extension)
# This produces:
#   libfoip_direct_demo.a       (Linux)
#   foip_direct_demo.lib        (Windows)
set_target_properties(foip_direct_demo_static PROPERTIES
    OUTPUT_NAME foip_direct_demo
)

# ---------------------------------------------------------
# Optional executable (uncomment if needed)
# ---------------------------------------------------------
# add_executable(foip_demo_app src/main.cpp)
# target_link_libraries(foip_demo_app PRIVATE foip_direct_demo)
