<div align="center">
    <a href="https://github.com/zpl-c/zpl"><img src="https://user-images.githubusercontent.com/2182108/111983468-d5593e80-8b12-11eb-9c59-8c78ecc0504e.png" alt="zpl" /></a>
</div>

<br />

<div align="center">
    <a href="https://github.com/zpl-c/track/actions"><img src="https://img.shields.io/github/actions/workflow/status/zpl-c/track/tester.yml?branch=main&label=Tests&style=for-the-badge" alt="Build status" /></a>
    <a href="https://discord.gg/2fZVEym"><img src="https://img.shields.io/discord/354670964400848898?color=7289DA&style=for-the-badge" alt="discord" /></a>
</div>

<br />
<div align="center">
  Event tracking library.
</div>

<div align="center">
  <sub>
    Brought to you by <a href="https://github.com/zpl-zak">@zpl-zak</a>
    and <strong>contributors</strong>
  </sub>
</div>

# Introduction
zpl.track is a cross-platform event tracker that can send your event data to a UDP echo server. You can then pipe the telemetry data to any destinations desired. It aims to be very minimalist and easy to integrate.

# How to obtain this library?

There are several options in getting hands-on this library:

## 1. Via CLI (the simplest option)
```sh
curl -L https://github.com/zpl-c/track/releases/latest/download/track.h > track.h
# OR
wget https://github.com/zpl-c/track/releases/latest/download/track.h -O track.h
```

This approach will download the latest release of zpl.track with everything prepared for you.

## 2. Via GitHub releases page
* Navigate to the [releases page](https://github.com/zpl-c/track/releases) of the zpl.track repository and pick the version you want to use. Each release contains the distributed version of zpl.track for you to use easily.

## 3. Cloning the repository
* Clone the repository by pressing the **Clone or download** button for instructions or use your terminal to clone:
```sh
git clone https://github.com/zpl-c/track.git
```

We use this method to develop zpl.track as it contains all the development sources that are easy to modify and expand, but it is not suitable for use in production.

Please follow the [CONTRIBUTING.md](.github/CONTRIBUTING.md) guide to learn how to contribute to the project.

## Build the demo
We use CMake to generate project files.
You can do the following on the command line to create and build this project:
```sh
cmake -S misc -B build
cmake --build build
```

Run `build/echo` first, then run the compiled `build/demo`.

## NodeJS event receiver companion
We offer a simple to use NodeJS library to receive telemetry data from zpl.track. You can check it out on [npm registry](https://www.npmjs.com/package/zpl.track) or by installing the package locally using:
```sh
npm install --save zpl.track
# OR
yarn install zpl.track
```

## Example code

```c
#define TRACK_IMPL
#include <track.h>

#include <stdlib.h> // for rand, srand
#include <time.h> // for time
#include <string.h> // for printf, snprintf

int err = 0;

#define CHECK(code)\
    err = code;\
    if (err) {\
        printf("error code: %d\n", err);\
        track_destroy();\
        return err;\
    }

int main() {
    srand(time(NULL)*time(NULL));
    CHECK(track_init("127.0.0.1", "8200"));

    char userId[13] = {0};
    snprintf(userId, 13, "id-%d", rand()%99999);

    CHECK(track_ident(userId, "{\"demoId\": 42, \"name\": \"Jane Doe\"}"));
    CHECK(track_event("demo_app opened", userId, "{\"foo\": 123, \"open_timestamp\": 123893893}"));
    CHECK(track_event("demo_app action", userId, "{\"foo\": \"bar\"}"));
    CHECK(track_group(userId, "abc", "{\"demoId\": 42, \"name\": \"Jane Doe\"}"));

    track_prop props[] = {
        {"mode", "\"demo\""},
        {"speed", "\"fast\""},
        {"contains_bugs", "false"},
        {0}
    };

    CHECK(track_event_props("demo_app submitted", userId, props));

    CHECK(track_destroy());
    return 0;
}
```

## Looking for the old Segment.IO version?
We discontinued the outdated library due to its complexity and reliance on cURL and CMake. We made modifications and streamlined the API for simplicity and performance. The old version can still be accessed [here](https://github.com/zpl-c/track/tree/v1), but we don't provide support for it.

## License

See [LICENSE](LICENSE) for the license.
