#pragma once
#include "track_system.h"

typedef enum {
    TRACK_HTTP_MSGKIND_TRACK,
    TRACK_HTTP_MSGKIND_IDENTIFY,
    TRACK_HTTP_MSGKIND_GROUP,
} track_module_http_msg_kind;

#define TRACK_MODULE_HTTP_EVENT(name) int name(track_module_http_msg_kind msg_kind, char const *event_id, char const *user_id, char const *group_id, char const *data, const char *raw_data, void **headers)
typedef TRACK_MODULE_HTTP_EVENT(track_module_http_event_proc);

TRACK_API int track_module_http_register(track_module_http_event_proc *proc);
TRACK_API int track_module_http_send(char const *url, char const *post, void **headers);
TRACK_API void track_module_http_add_header(void **headers, char const *header);
