#pragma once

#define TRACK_MODULE_HTTP_PAYLOAD(name) char *name(char const *event_id, char const *user_id, char const *data, void **headers)
typedef TRACK_MODULE_HTTP_PAYLOAD(track_module_http_payload_proc);

int track_module_http_register(char const *url, track_module_http_payload_proc *proc);
void track_module_http_add_header(void **headers, char const *header);