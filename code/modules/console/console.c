#include "console.h"
#include "track.h"
#include <stdio.h>
#include <time.h>

#define ZPL_NANO
#include "zpl.h"

TRACK_EVENT_PROC(console__event_handler) {
    if (!user_data)
        user_data = "track_module_console";
    char datetime[80];
    time_t now = time(0);
    struct tm *tm_now = localtime(&now);
    strftime(datetime, 80, "%c", tm_now);
    printf("[%s : %s] %s@'%s' -- \"%s\"\n", (char const *)user_data, datetime, user_id, event_id, data);
    return 0;
}

int track_module_console_register(char const *prefix) {
    return track_module_register(console__event_handler, 0, (void*)prefix);
}