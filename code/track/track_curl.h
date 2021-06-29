#pragma once
#include "track_system.h"

TRACK_API int track_curl_init(int is_async);
TRACK_API int track_curl_destroy(void);

TRACK_API int track_curl_send_message(void *handle);
TRACK_API int track_curl_poll(void);
TRACK_API int track_curl_perform(void);
