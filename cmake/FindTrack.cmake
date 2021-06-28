# Include this file to pull in the entire library.
find_package(track QUIET)

if (NOT track_FOUND)
    include(FetchContent)

    FetchContent_Declare(
        track
        URL https://github.com/zpl-c/track/archive/main.tar.gz
    )

    set(TRACK_BUILD_DEMO OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(track)
    add_library(track::track ALIAS track)
    include_directories(${CMAKE_BINARY_DIR}/_deps/track-src/code)
endif()

macro(link_track_library target)
    target_link_libraries(target track::track)

    if (NOT UNIX)
        target_link_libraries(target CURL::libcurl)
    endif ()
endmacro()
