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
