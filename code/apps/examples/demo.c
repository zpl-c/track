#define TRACK_IMPL
#include <track.h>

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
    CHECK(track_init("127.0.0.1", "8200"));

    CHECK(track_ident("0421", "demo,quick"));
    CHECK(track_event("open_demo_app", "0421", "{\\\"foo\\\": 123}"));
    CHECK(track_event("do_something", "0421", "{\\\"foo\\\": \\\"bar\\\"}"));
    CHECK(track_group("0421", "abc", "demo,quick"));

    CHECK(track_destroy());
    return 0;
}
