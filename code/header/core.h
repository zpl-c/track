// file: header/core.h

#pragma once

//~ Options
#ifndef TRACK_SEND_BUFSIZE
#define TRACK_SEND_BUFSIZE 577
#endif

//~ Errors
#define TRACK_ERROR_INIT_FAIL      1
#define TRACK_ERROR_SOCKET_FAIL    2
#define TRACK_ERROR_SOCKET_INVALID 3
#define TRACK_ERROR_BUFFER_FULL    4
#define TRACK_ERROR_SEND_FAIL      5

//~ Lifecycle methods
TRACK_DEF int track_init(char const *host, char const *port);
TRACK_DEF int track_destroy(void);

//~ Event tracking
TRACK_DEF int track_event(char const *event_id, char const *user_id, char const *data);
TRACK_DEF int track_ident(char const *user_id, char const *traits);
TRACK_DEF int track_group(char const *user_id, char const *group_id, char const *traits);
