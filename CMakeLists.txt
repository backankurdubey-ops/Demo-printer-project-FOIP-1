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

# Create a shared library (.so)
add_library(foip_direct_demo SHARED ${SOURCES})

# Optional: specify library version
set_target_properties(foip_direct_demo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# If your code has a main.cpp and you do NOT want an exe, comment it out:
# add_executable(foip_demo_app src/main.cpp)
# target_link_libraries(foip_demo_app PRIVATE foip_direct_demo)
