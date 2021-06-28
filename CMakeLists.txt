cmake_minimum_required(VERSION 3.11)
project(track)
include(cmake/utils.cmake)
include(GNUInstallDirs)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}//${CMAKE_BUILD_TYPE}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(TRACK_BUILD_DEMO "BUILD THE DEMO APPLICATION" ON)
option(TRACK_CURL_SUPPORT "ENABLE CURL SUPPORT FOR MODULES" ON)

add_definitions("-DCURL_STATICLIB")

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_compile_options(/MP /W4)
endif()

include_directories(code)
include_directories(code/vendors)

add_subdirectory(code)
add_subdirectory(code/modules)

if (TRACK_BUILD_DEMO)
    add_subdirectory(code/demo)
endif()
