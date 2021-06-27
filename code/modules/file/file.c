#include "file.h"
#include "track.h"
#include <stdio.h>
#include <stdlib.h>

#define ZPL_NANO
#define ZPL_ENABLE_PARSER
#include "zpl.h"

#include "track_utils.h"

TRACK_EVENT_PROC(file__event_handler) {
    track_escaped_string e = track_escape_string(event_id, "\"", '"');
    track_escaped_string u = track_escape_string(user_id, "\"", '"');
    track_escaped_string t = track_escape_string(data, "\"", '"');
    fprintf((FILE*)user_data, "%lld,\"%.*s\",\"%.*s\",\"%.*s\"\n", zpl_time_win32_to_unix(zpl_time_utc_ms()), e.len, e.text, u.len, u.text, t.len, t.text);
    zpl_file_close(&e.f);
    zpl_file_close(&u.f);
    zpl_file_close(&t.f);
    return 0;
}

TRACK_IDENT_PROC(file__ident_handler) {
    track_escaped_string u = track_escape_string(user_id, "\"", '"');
    track_escaped_string t = track_escape_string(traits, "\"", '"');
    fprintf((FILE*)user_data, "%lld,\"%s\",\"%.*s\",\"%.*s\"\n", zpl_time_win32_to_unix(zpl_time_utc_ms()), "Identify", u.len, u.text, t.len, t.text);
    zpl_file_close(&u.f);
    zpl_file_close(&t.f);
    return 0;
}

TRACK_GROUP_PROC(file__group_handler) {
    track_escaped_string u = track_escape_string(user_id, "\"", '"');
    track_escaped_string g = track_escape_string(group_id, "\"", '"');
    track_escaped_string t = track_escape_string(traits, "\"", '"');
    fprintf((FILE*)user_data, "%lld,\"Group %.*s\",\"%.*s\",\"%.*s\"\n", zpl_time_win32_to_unix(zpl_time_utc_ms()), g.len, g.text, u.len, u.text, t.len, t.text);
    zpl_file_close(&u.f);
    zpl_file_close(&g.f);
    zpl_file_close(&t.f);
    return 0;
}

TRACK_MODULE_UNREGISTER_PROC(file__unregister_handler) {
    fclose((FILE*)user_data);
    return 0;
}

int track_module_file_register(char const *filename) {
    FILE *f = fopen(filename, "ab");
    if (!f) return -1;
    
    return track_module_register(file__event_handler, file__ident_handler, file__group_handler, file__unregister_handler, (void*)f);
}