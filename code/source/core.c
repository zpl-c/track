// file: source/core.c

#include <string.h> // for strlen, memcpy

int32_t track__sock = -1;

//~ Lifecycle methods
int track_init(char const *host, char const *port) {
    if (track__sock > -1) {
        swrapClose(track__sock);
        track__sock = -1;
    }

    track_destroy();

    if (swrapInit())
        return -TRACK_ERROR_INIT_FAIL;

    track__sock = swrapSocket(SWRAP_UDP, SWRAP_CONNECT, SWRAP_DEFAULT, host, port);

    if (track__sock == -1) {
        track_destroy();
        return -TRACK_ERROR_SOCKET_FAIL;
    }

    return 0;
}
int track_destroy(void) {
    swrapTerminate();
    track__sock = -1;
    return 0;
}

//~ Buffer utilities
static char track__buffer[TRACK_SEND_BUFSIZE];
static int32_t track__buffer_len = 0;
static int track__errno = 0;

static void track__buffer_flush(void) {
    track__buffer_len = 0;
}

static int track__buffer_appendc(char const *str) {
    int32_t size = strlen(str);
    if (track__buffer_len+size >= TRACK_SEND_BUFSIZE)
        return -TRACK_ERROR_BUFFER_FULL;
    memcpy(track__buffer+track__buffer_len, str, size);
    track__buffer_len += size;
    return 0;
}

#define TRACK__APPEND_SAFE(xx)\
    track__errno = track__buffer_appendc(xx);\
    if (track__errno) return track__errno;

//~ Event tracking
int track_event(char const *event_id, char const *user_id, char const *data) {
    if (track__sock == -1)
        return -TRACK_ERROR_SOCKET_INVALID;
    track__buffer_flush();

    TRACK__APPEND_SAFE("{\"userId\":\"");
    TRACK__APPEND_SAFE(user_id);
    TRACK__APPEND_SAFE("\",\"event\":\"");
    TRACK__APPEND_SAFE(event_id);
    TRACK__APPEND_SAFE("\",\"properties\":\"");
    TRACK__APPEND_SAFE(data);
    TRACK__APPEND_SAFE("\"}");

    if (!swrapSend(track__sock, track__buffer, track__buffer_len))
        return -TRACK_ERROR_SEND_FAIL;
    return 0;
}

int track_ident(char const *user_id, char const *traits) {
    if (track__sock == -1)
        return -TRACK_ERROR_SOCKET_INVALID;

    track__buffer_flush();

    TRACK__APPEND_SAFE("{\"userId\":\"");
    TRACK__APPEND_SAFE(user_id);
    TRACK__APPEND_SAFE("\",\"traits\":\"");
    TRACK__APPEND_SAFE(traits);
    TRACK__APPEND_SAFE("\"}");

    if (!swrapSend(track__sock, track__buffer, track__buffer_len))
        return -TRACK_ERROR_SEND_FAIL;
    return 0;
}

int track_group(char const *user_id, char const *group_id, char const *traits) {
    if (track__sock == -1)
        return -TRACK_ERROR_SOCKET_INVALID;

    track__buffer_flush();
    
    TRACK__APPEND_SAFE("{\"userId\":\"");
    TRACK__APPEND_SAFE(user_id);
    TRACK__APPEND_SAFE("\",\"groupId\":\"");
    TRACK__APPEND_SAFE(group_id);
    TRACK__APPEND_SAFE("\",\"traits\":\"");
    TRACK__APPEND_SAFE(traits);
    TRACK__APPEND_SAFE("\"}");

    if (!swrapSend(track__sock, track__buffer, track__buffer_len))
        return -TRACK_ERROR_SEND_FAIL;
    return 0;
}

#undef TRACK__APPEND_SAFE
