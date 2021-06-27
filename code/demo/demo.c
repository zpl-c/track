// Example project showcasing the use of zpl-track

#include "track.h"
#include "track_curl.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// modules
#include "modules/console/console.h"
#include "modules/file/file.h"
#include "modules/http/http.h"
#include "modules/segment/segment.h"

TRACK_EVENT_PROC(custom_event_handler) {
    (void)user_data;
    printf("%s triggers %s with data '%s'\n", user_id, event_id, data);
    return 0;
}

TRACK_MODULE_HTTP_EVENT(my_http_event) {
    (void)raw_data;
    (void)msg_kind;
    (void)group_id;
    static char buf[4096];
    snprintf(buf, 4096, "{ \"event\": \"%s\", \"user\": \"%s\", \"data\": \"%s\" }", event_id, user_id, data);
    return track_module_http_send("http://127.0.0.1:8200/", buf, headers);
}

int main(void) {
    char *segment_key = getenv("SEGMENT_WRITE_KEY");
    if (!segment_key) segment_key = "abc123";
    track_init(1);
    track_module_console_register("demo log");
    track_module_file_register("telemetry.csv");
    track_module_segment_register(segment_key);
    track_module_register(custom_event_handler, 0, 0, 0, 0);
    
    // run tools/echo in node.js to test HTTP endpoint
    track_module_http_register(my_http_event);
    
    // identify the user with associated traits
    track_ident("965d026a-2035-4671-92d6-3bc384ecede4", "{\"servername\": \"foo\" }");
    
    // group the user with a specific entity
    track_group("965d026a-2035-4671-92d6-3bc384ecede4", "31ceee10-3dc8-402f-a8d1-08c89ce4ef79", "{\"role\": \"community-server\" }");
    
    // wait for these events to pass first
    track_curl_perform();
    
    // sample events
    {
        track_event("Server Join", "965d026a-2035-4671-92d6-3bc384ecede4", "{\"nickname\": \"joe\" }");
        track_event("Server Leave", "965d026a-2035-4671-92d6-3bc384ecede4", "{\"nickname\": \"joe\", \"reason\": \"ban\" }");
        track_event("Server Verify Cache", "965d026a-2035-4671-92d6-3bc384ecede4", "{\"cache-size\": 1536}");
    }
    
    // wait until all requests are processed
    track_curl_perform();
    
    track_destroy();
    return 0;
}
