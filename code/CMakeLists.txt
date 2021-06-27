include(GNUInstallDirs)
include_directories(.)

add_library(track STATIC track.c)

if (TRACK_CURL_SUPPORT)
  setup_curl()
	include_directories(${CMAKE_BINARY_DIR}/_deps/curl-src/include)
	add_library(track-curl STATIC track_curl.c)
else()
	add_definitions(-DTRACK_DISABLE_CURL_SUPPORT)
endif()

install(TARGETS track DESTINATION ${CMAKE_INSTALL_BINDIR})
