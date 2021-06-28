project(track C)
include(GNUInstallDirs)

add_library(track STATIC)
target_sources(track
  PRIVATE
    track/track.c
    track/track_curl.c

    track/track_module_file.c
    track/track_module_console.c
    track/track_module_http.c
    track/track_module_segment.c
)

target_include_directories(track
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/track
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/code>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

file(GLOB public_headers "track/*.h" "vendors/*.h")
set_target_properties(track PROPERTIES PUBLIC_HEADER "${public_headers}")

install(TARGETS track
    EXPORT "trackConfig"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/track
)

install(
    EXPORT "trackConfig"
    FILE "trackConfig.cmake"
    NAMESPACE track::
    DESTINATION cmake
)

find_package(CURL QUIET)
if(NOT CURL_FOUND)
  setup_curl()
  include_directories(${CMAKE_BINARY_DIR}/_deps/curl-src/include)
  set(CURL_LIBRARIES libcurl PARENT_SCOPE)
else()
  message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
  message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
  message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
  include_directories(${CURL_INCLUDE_DIRS})
endif()
target_link_libraries(track PUBLIC ${CURL_LIBRARIES})
