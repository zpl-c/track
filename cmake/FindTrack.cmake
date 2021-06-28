# Include this file to pull in the entire library.
find_package(track QUIET)

if (NOT track_FOUND)
    set(CPM_DOWNLOAD_VERSION 0.32.2)

    if(CPM_SOURCE_CACHE)
    # Expand relative path. This is important if the provided path contains a tilde (~)
    get_filename_component(CPM_SOURCE_CACHE ${CPM_SOURCE_CACHE} ABSOLUTE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    endif()

    if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
    endif()

    include(${CPM_DOWNLOAD_LOCATION})
    set(TRACK_BUILD_DEMO OFF CACHE BOOL "" FORCE)
    CPMAddPackage(
        NAME track
        GIT_REPOSITORY https://github.com/zpl-c/track.git
        GIT_TAG main
    )
    add_library(track::track ALIAS track)
    include_directories(${CMAKE_BINARY_DIR}/_deps/track-src/code)
endif()
