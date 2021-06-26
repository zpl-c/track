#include "console.h"
#include "track.h"
#include <stdio.h>

#define ZPL_NANO
#include "zpl.h"

TRACK_EVENT_PROC(console__event_handler) {
    (void)user_data;
    // TODO(zaklaus): convert timestamp to date-time
    printf("[%lld] %s@'%s' -- \"%s\"\n", zpl_time_win32_to_unix(zpl_time_utc_ms()), user_id, event_id, data);
    return 0;
}

int track_module_console_register(void) {
    return track_module_register(console__event_handler, 0, 0);
}