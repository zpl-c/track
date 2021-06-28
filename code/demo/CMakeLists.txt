add_executable(track-demo
	demo.c
)

target_link_libraries(track-demo CURL::libcurl track)
