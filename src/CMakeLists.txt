option(BUILD_SHARED_LIBS "Build udp_streamer shared library" OFF)

add_library(udp_streamer)

target_compile_features(udp_streamer PUBLIC cxx_std_11)

target_link_libraries(udp_streamer
  PUBLIC
    simple_socket
    ${OpenCV_LIBS})

target_include_directories(
    udp_streamer
  PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>"
    ${OpenCV_INCLUDE_DIRS}
    $<TARGET_PROPERTY:simple_socket,INTERFACE_INCLUDE_DIRECTORIES>
)

target_sources(
    udp_streamer
  PRIVATE 
    udp_streamer.cpp
)

install(
  TARGETS udp_streamer
  EXPORT udp_streamerTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/udp_streamer
  DESTINATION include
)

install(
  EXPORT udp_streamerTargets
  FILE udp_streamerTargets.cmake
  NAMESPACE udp_streamer::
  DESTINATION lib/cmake/udp_streamer
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/udp_streamerConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/udp_streamer
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/udp_streamerConfigVersion.cmake"
  VERSION "${udp_streamer_VERSION_MAJOR}.${udp_streamer_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(
  FILES 
    "${CMAKE_CURRENT_BINARY_DIR}/udp_streamerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/udp_streamerConfigVersion.cmake"
  DESTINATION lib/cmake/udp_streamer
)

export(
  EXPORT udp_streamerTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/udp_streamerTargets.cmake"
  NAMESPACE udp_streamer::
)