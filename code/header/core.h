// file: header/core.h

#pragma once

//~ Options
#ifndef TRACK_SEND_BUFSIZE
#define TRACK_SEND_BUFSIZE 576
#endif

//~ Errors
#define TRACK_ERROR_INIT_FAIL      1
#define TRACK_ERROR_SOCKET_FAIL    2
#define TRACK_ERROR_SOCKET_INVALID 3
#define TRACK_ERROR_BUFFER_FULL    4
#define TRACK_ERROR_SEND_FAIL      5
#define TRACK_ERROR_INPUT_INVALID  6

//~ Lifecycle methods
TRACK_DEF int track_init(char const *host, char const *port);
TRACK_DEF int track_destroy(void);

//~ Event tracking
TRACK_DEF int track_event(char const *event_id, char const *user_id, char const *json_payload);
TRACK_DEF int track_ident(char const *user_id, char const *traits);
TRACK_DEF int track_group(char const *user_id, char const *group_id, char const *traits);

//~ Event utilities

typedef struct {
  char const *key;
  char const *val;
} track_prop;

// props array terminates on first key set to NULL
TRACK_DEF int track_event_props(char const *event_id, char const *user_id, const track_prop *props);
