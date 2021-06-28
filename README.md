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

```

## Build the demo
We use CMake to generate project files.
You can do the following on the command line to create and build this project:
```sh
git clone https://github.com/zpl-c/track.git
cd track
cmake -B build
cmake --build build
```

It will download all the dependencies and build the demo.

Run `tools/echo` in node.js and then run the compiled demo.

## Install the library
Make sure to set `CMAKE_PREFIX_PATH` to desired install location and then run the following commands:

```sh
git clone https://github.com/zpl-c/track.git
cd track
cmake -B build -DTRACK_BUILD_DEMO=OFF
cmake --build build --target install
```

This will build and install the library and its dependencies to the desired install dir.

## How to use in your project
You can include the [FindTrack](cmake/FindTrack.cmake) CMake file to download and build zpl.track within your project. It pulls the latest version of the library and builds all its dependencies automatically, all that's left for you to do is to link against the library and its modules, like:

```sh
cmake_minimum_required(VERSION 3.11)
project(demo)

# FindTrack either finds the library using find_package() call or downloads the library sources and builds it locally.
include(cmake/FindTrack.cmake)

add_executable(demo-app main.c)
target_link_libraries(demo-app track::track)
```

You can also specify your `CMAKE_PREFIX_PATH` so that `FindTrack` gets a chance to use a local copy of the library.

See demo app for more information.

## License

See [LICENSE](LICENSE) for the license.
