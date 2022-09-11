cmake_minimum_required(VERSION 3.16)
project(server)

set (CMAKE_CXX_STANDARD 20)

include_directories(BEFORE ../lib)

add_library(streamer
    ../lib/tcpsocket.cpp
    ../lib/socketaddress.cpp
    ../lib/fileinputstream.cpp
    ../lib/fileoutputstream.cpp
    ../lib/memoryinputstream.cpp
    ../lib/memoryoutputstream.cpp)

add_executable(server
    main.cpp
    tcpserver.cpp
    tcpclient.cpp
    streamserver.cpp
    directoryinfo.cpp
    wavfile.cpp)
target_link_libraries(server PRIVATE pthread streamer)
