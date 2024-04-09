cmake_minimum_required(VERSION 3.8)

project(udp_streamer VERSION 1.0 DESCRIPTION "A library for capturing images on one PC and transferring them to another via UDP with minimal delays" LANGUAGES CXX)

option(Demo "Demo" OFF)

include(GNUInstallDirs)

find_package(OpenCV REQUIRED)

add_subdirectory(lib)
add_subdirectory(src)

if (Demo)
    add_subdirectory(demo)
endif()
