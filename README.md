<div align="center">
    <a href="https://github.com/zpl-c/zpl"><img src="https://user-images.githubusercontent.com/2182108/111983468-d5593e80-8b12-11eb-9c59-8c78ecc0504e.png" alt="zpl" /></a>
</div>

<br />

<div align="center">
    <a href="https://discord.gg/2fZVEym"><img src="https://img.shields.io/discord/354670964400848898?color=7289DA&style=for-the-badge" alt="discord" /></a>
</div>

<br />
<div align="center">
  Event tracking library.
</div>

<div align="center">
  <sub>
    Brought to you by <a href="https://github.com/zaklaus">@zaklaus</a>
    and <strong>contributors</strong>
  </sub>
</div>

# Introduction
zpl.track is a cross-platform event tracker that can send your event data to provided destinations. It validates your JSON data and provides an easy way to extend your endpoints.

## Disclaimer

This library is still in development so no support will be provided. Please, report any issues you have found.

## Example code

```c
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

```

## Build the demo
We use CMake to generate project files.
You can do the following on the command line to create and build this project:
```sh
git clone https://github.com/zpl-c/track.git
cd track
cmake -S . -B build
cmake --build build
```

It will download all the dependencies and build the demo.

Run `tools/echo` in node.js and then run the compiled demo.

## How to use in your project
Ideally, you should clone this repo and define the following in your CMake project:

```sh
set(TRACK_BUILD_DEMO OFF)
add_subdirectory(vendors/track)
```

then link against `track` and all used modules and their dependencies. See demo app for more information.

## License

See [LICENSE](LICENSE) for the license.
