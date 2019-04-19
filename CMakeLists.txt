cmake_minimum_required(VERSION 3.0)

project(SocketAsio)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOOST_ASIO_STANDALONE -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ASIO_STANDALONE -std=c++11")

# Options
option(TARGET_WIN10 "Build For Windows 10" ON)
if (NOT DEFINED TARGET_WIN10)
    set(TARGET_WIN10 ON)
endif()

if (TARGET_WIN10 STREQUAL "ON")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0A00")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0A00")
endif()

# Target

# Test Target
add_executable(
    test_asio
    test/test_asio.cpp
)

target_include_directories(
    test_asio
    PRIVATE
    asio/include
)