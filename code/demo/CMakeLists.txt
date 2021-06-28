add_executable(track-demo
	demo.c
)

include_directories(.)

message(MESSAGE "Using curl libraries: ${CURL_LIBRARIES}")
target_link_libraries(track-demo ${CURL_LIBRARIES} track track-console track-file track-curl track-http track-segment)
