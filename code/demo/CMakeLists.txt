add_executable(track-demo
	demo.c
)

include_directories(.)
target_link_libraries(track-demo ${CURL_LIBS} track track-console track-file track-curl track-http track-segment)

link_system_libs(track-demo)
