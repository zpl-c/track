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

TRACK_MODULE_HTTP_PAYLOAD(my_http_payload) {
    (void)headers;
    char *buf = (char *)malloc(4096);
    if (!buf) return 0;
    snprintf(buf, 4096, "{ \"event\": \"%s\", \"user\": \"%s\", \"data\": \"%s\" }", event_id, user_id, data);
    
    return buf;
}

int main(void) {
    char *segment_key = getenv("SEGMENT_WRITE_KEY");
    if (!segment_key) segment_key = "abc123";
    track_init(1);
    track_module_console_register("demo log");
    track_module_file_register("telemetry.csv");
    track_module_segment_register(segment_key);
    track_module_register(custom_event_handler, 0, 0);
    
    // run tools/echo in node.js to test HTTP endpoint
    track_module_http_register("http://127.0.0.1:8200/", my_http_payload);
    
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
