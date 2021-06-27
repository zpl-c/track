add_subdirectory(console)
add_subdirectory(file)

if (TRACK_CURL_SUPPORT)
  add_subdirectory(http)
  add_subdirectory(segment)
endif()
