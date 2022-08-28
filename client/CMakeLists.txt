cmake_minimum_required(VERSION 3.16)
project(client)

set (CMAKE_CXX_STANDARD 20)

include_directories(BEFORE ../lib)

add_library(streamer
    ../lib/fileinputstream.cpp
    ../lib/fileoutputstream.cpp
    ../lib/memoryoutputstream.cpp
    ../lib/memoryinputstream.cpp
    ../lib/tcpsocket.cpp
    ../lib/socketaddress.cpp)

add_executable(client
    main.cpp
    wavplayer.cpp
    streamerclient.cpp)
target_link_libraries(client PRIVATE asound pthread streamer)
