add_library(track-http STATIC http.c)
include_directories(.)
include_directories(${CMAKE_BINARY_DIR}/_deps/curl-src/include)
