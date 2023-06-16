<div align="center">
    <a href="https://github.com/zpl-c/zpl"><img src="https://user-images.githubusercontent.com/2182108/111983468-d5593e80-8b12-11eb-9c59-8c78ecc0504e.png" alt="zpl" /></a>
</div>

<br />

<div align="center">
    <a href="https://github.com/zpl-c/skeleton/actions"><img src="https://img.shields.io/github/workflow/status/zpl-c/track/tester?label=Tests&style=for-the-badge" alt="Build status" /></a>
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
zpl.track is a cross-platform event tracker that can send your event data to a UDP echo server. You can then pipe the telemetry data to any destinations desired. It aims to be very minimalist and easy to integrate.

## Disclaimer

This library is still in development so no support will be provided. Please, report any issues you have found.

## Example code

```c
#define TRACK_IMPL
#include <track.h>

#include <string.h> // for printf, itoa

int err = 0;

#define CHECK(code)\
    err = code;\
    if (err) {\
        printf("error code: %d\n", err);\
        track_destroy();\
        return err;\
    }

int main() {
    CHECK(track_init("127.0.0.1", "8200"));
    CHECK(track_ident("0421", "demo,quick"));
    CHECK(track_event("open_demo_app", "0421", "1234"));
    CHECK(track_group("0421", "abc", "demo,quick"));

    for (int i = 0; i < 999; i++) {
        static char buf[4]={0};
        itoa(i, buf, 10);
        CHECK(track_event("demo_counter", "0421", buf));
    }

    CHECK(track_destroy());
    return 0;
}
```

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

### Build the demo
We use CMake to generate project files.
You can do the following on the command line to create and build this project:
```sh
cmake -S misc -B build
cmake --build build
```

Run `build/echo` first, then run the compiled `build/demo`.

## License

See [LICENSE](LICENSE) for the license.
