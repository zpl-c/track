add_library(track-segment STATIC segment.c)
include_directories(.)
include_directories(${CMAKE_BINARY_DIR}/_deps/curl-src/include)
