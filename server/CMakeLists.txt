cmake_minimum_required(VERSION 3.16)
project(server)

set (CMAKE_CXX_STANDARD 20)

include_directories(BEFORE ../lib)

file(GLOB libsources ../lib/*.cpp)
add_library(streamerlib ${libsources})

file(GLOB serversources ./*.cpp)
add_executable(streamerserver ${serversources})
target_link_libraries(streamerserver PRIVATE pthread streamerlib)
