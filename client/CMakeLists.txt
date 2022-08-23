cmake_minimum_required(VERSION 3.16)
project(client)

include_directories(BEFORE ../lib)

file(GLOB libsources ../lib/*.cpp)
add_library(streamerlib ${libsources})

file(GLOB clientsources ./*.cpp)
add_executable(streamerclient ${clientsources})
target_link_libraries(streamerclient PRIVATE asound pthread streamerlib)
