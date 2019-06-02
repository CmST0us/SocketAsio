cmake_minimum_required(VERSION 3.0)

project(SocketAsio)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DASIO_STANDALONE -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD c99)

include_directories(
    asio/asio/include
)

if (WIN32)
    link_libraries(
        ws2_32
    )
else()
    link_libraries(
        pthread
    )
endif()

# Options
option(TARGET_WIN10 "Build For Windows 10" OFF)
if (NOT DEFINED TARGET_WIN10)
    message(STATUS "Build For Windows10")
    set(TARGET_WIN10 ON)
endif()

if (TARGET_WIN10 STREQUAL "ON")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0A00")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0A00")
endif()

option(BUILD_FOR_IOS "Build For iOS" OFF)
if (NOT DEFINED BUILD_FOR_IOS)
    set(BUILD_FOR_IOS OFF)
endif()

if(BUILD_FOR_IOS STREQUAL "ON")
    set(CMAKE_XCODE_ATTRIBUTE_SDKROOT "iphoneos")
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0")
endif()

set(CMAKE_INSTALL_PREFIX "libSocketAsio")
# Target
aux_source_directory(src/TCP SOCKETASIO_TCP_SOURCE)
aux_source_directory(src/UDP SOCKETASIO_UDP_SOURCE)
add_library(
    SocketAsio
    ${SOCKETASIO_TCP_SOURCE}
    ${SOCKETASIO_UDP_SOURCE}
)

target_include_directories(
    SocketAsio
    PUBLIC
    include/libSocketAsio
)

install(
    TARGETS SocketAsio
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(
    DIRECTORY include/libSocketAsio
    DESTINATION include
)

# Test Target
add_executable(
    test_asio
    test/test_asio.cpp

    src/TCP/TCPAcceptor.cpp
    src/TCP/TCPConnector.cpp
    src/TCP/TCPSocket.cpp
    src/UDP/UDPSocket.cpp
)
target_include_directories(test_asio PRIVATE include/libSocketAsio)
