cmake_minimum_required(VERSION 3.8)

project(udp_streamer VERSION 1.0 DESCRIPTION "A library for capturing images on one PC and transferring them to another via UDP with minimal delays" LANGUAGES CXX)

include(GNUInstallDirs)

find_package(OpenCV REQUIRED)

add_subdirectory(lib)

add_subdirectory(src)

add_executable(udp_transmitter src/udp_transmitter.cpp)
target_link_libraries(udp_transmitter PRIVATE udp_streamer)

add_executable(udp_receiver src/udp_receiver.cpp)
target_link_libraries(udp_receiver PRIVATE udp_streamer)