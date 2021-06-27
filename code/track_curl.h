#pragma once

int track_curl_init(int is_async);
int track_curl_destroy(void);

int track_curl_send_message(void *handle);
int track_curl_poll(void);
int track_curl_perform(void);