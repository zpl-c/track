# Include this file to pull in the entire library.
find_package(track QUIET)

if (NOT track_FOUND)
    include(FetchContent)

    FetchContent_Declare(
        track
        URL https://github.com/zpl-c/track/archive/main.tar.gz
    )

    set(BUILD_OPENSSL_SRC_DIR ${CMAKE_BINARY_DIR}/_deps/track-src/code/vendors/build-openssl)
    set(TRACK_BUILD_DEMO OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(track)
    include_directories(${CMAKE_BINARY_DIR}/_deps/track-src/code)
endif()
