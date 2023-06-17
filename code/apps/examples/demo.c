#define TRACK_IMPL
#include <track.h>

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
    srand(time(NULL));
    CHECK(track_init("127.0.0.1", "8200"));

    char userId[13] = {0};
    snprintf(userId, 13, "id-%d", rand()%9999);

    CHECK(track_ident(userId, "{\\\"demoId\\\": 42, \\\"name\\\": \\\"Jane Doe\\\"}"));
    CHECK(track_event("demo_app opened", userId, "{\\\"foo\\\": 123, \\\"open_timestamp\\\": 123893893}"));
    CHECK(track_event("demo_app action", userId, "{\\\"foo\\\": \\\"bar\\\"}"));
    CHECK(track_group(userId, "abc", "{\\\"demoId\\\": 42, \\\"name\\\": \\\"Jane Doe\\\"}"));

    CHECK(track_destroy());
    return 0;
}
