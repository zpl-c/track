function(link_system_libs target_name)
    if (WIN32)
        target_link_libraries(${target_name} winmm comdlg32)
    elseif (APPLE)
        target_link_libraries(${target_name} pthread m dl)
    elseif (UNIX)
        target_link_libraries(${target_name} pthread m dl atomic)
    endif()
endfunction()

macro(use_cxx11)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        endif ()
    else ()
        set(CMAKE_CXX_STANDARD 11)
    endif ()
endmacro(use_cxx11)

macro(setup_curl)
    # Prefer static libs over shared lib
    LIST(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
    include(../cmake/CPM.cmake)

    OPTION(ENABLE_OPENSSL_FROM_SOURCE "BUILD OPENSSL FROM SOURCE AT CONFIGURE TIME" ON)

    if(ENABLE_OPENSSL_FROM_SOURCE)
    set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-openssl)
    if(NOT BUILD_OPENSSL_SRC_DIR)
        set(BUILD_OPENSSL_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors/build-openssl)
    endif()
    set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install/OpenSSL")
    file(MAKE_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR})
    if(NOT EXISTS ${BUILD_OPENSSL_INSTALL_DIR})
        message(STATUS "Building OpenSSL.. at ${BUILD_OPENSSL_WORKING_DIR}, Install DIR ${BUILD_OPENSSL_INSTALL_DIR}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} ${BUILD_OPENSSL_SRC_DIR} -DINSTALL_DIR=${BUILD_OPENSSL_INSTALL_DIR}
            WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
    else()
        message(STATUS "${BUILD_OPENSSL_INSTALL_DIR} already exists, skips rebuilding..")
    endif()

    set(OPENSSL_ROOT_DIR "${BUILD_OPENSSL_INSTALL_DIR}" CACHE INTERNAL "OPENSSL ROOT DIR")
    message(MESSAGE "Setting OPENSSL ROOT DIR to newly built OpenSSL ${OPENSSL_ROOT_DIR}")
    endif()

    if (MSVC)
    set(CURL_PLATFORM_OPTIONS "CMAKE_USE_SCHANNEL ON;CMAKE_USE_OPENSSL OFF")
    elseif(APPLE)
    set(CURL_PLATFORM_OPTIONS "CMAKE_USE_SECTRANSP OFF;CMAKE_USE_OPENSSL ON")
    else()
    set(CURL_PLATFORM_OPTIONS "CMAKE_USE_OPENSSL ON")
    endif()

    CPMAddPackage(
    NAME curl
    VERSION 7.77.0
    URL https://github.com/curl/curl/releases/download/curl-7_77_0/curl-7.77.0.tar.gz
    OPTIONS
    "CURL_STATICLIB ON"
    "BUILD_CURL_EXE OFF"
    "BUILD_SHARED_LIBS OFF"
    "BUILD_TESTING OFF"
    "WITH_STATIC_RT ON"
    "CMAKE_USE_LIBSSH2 OFF"
    ${CURL_PLATFORM_OPTIONS}
    )
endmacro()
