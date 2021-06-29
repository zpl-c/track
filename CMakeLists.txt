cmake_minimum_required(VERSION 3.11)
project(track)
include(cmake/utils.cmake)
include(GNUInstallDirs)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  set(TRACK_IS_MAIN TRUE)
else()
  set(TRACK_IS_MAIN FALSE)
endif()

if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()

option(TRACK_BUILD_DEMO "BUILD THE DEMO APPLICATION" ON)

add_definitions("-DCURL_STATICLIB" "-DTRACK_SHARED_LIB")

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_compile_options(/MP /W4)
endif()

include_directories(code)
include_directories(code/vendors)

add_subdirectory(code track)

if (TRACK_BUILD_DEMO)
  add_subdirectory(code/demo track-demo)
endif()
