#include "segment.h"
#include "track.h"
#include <stdio.h>
#include <stdlib.h>

#define ZPL_NANO
#define ZPL_ENABLE_PARSER
#define ZPL_ENABLE_HASHING
#include "zpl.h"

#include <curl/curl.h>

#include "track_utils.h"
#include "track_curl.h"

#define TRACK_MODULE_SEGMENT_TRACK_API "https://api.segment.io/v1/track"
#define TRACK_MODULE_SEGMENT_IDENTIFY_API "https://api.segment.io/v1/identify"
#define TRACK_MODULE_SEGMENT_GROUP_API "https://api.segment.io/v1/group"
#define TRACK_MODULE_SEGMENT_BUFSIZ 32768

typedef struct {
    char const *write_key;
} module_segment;

int track__module_segment_send(char const *url, char const *write_key, char const *req) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
    
    // TODO: Improve
    static char auth_pass[512];
    snprintf(auth_pass, 512, "%s:", write_key);
    zpl_u8 *auth_key = zpl_base64_encode(zpl_heap_allocator(), auth_pass, zpl_strlen(auth_pass));
    static char auth_token[512];
    snprintf(auth_token, 512, "Authorization: Basic %s", auth_key);
    zpl_mfree(auth_key);
    headers = curl_slist_append(headers, auth_token);
    
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, req);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    int res = track_curl_send_message(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "track_module_http curl error: %s\n", curl_easy_strerror(res));
        return -1;
    }
    
    return 0;
}

TRACK_EVENT_PROC(segment__event_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    track_escaped_string e = track_escape_string(event_id, "\"", '\\');
    track_escaped_string u = track_escape_string(user_id, "\"", '\\');
    
    module_segment *h = (module_segment*)user_data;
    
    static char buffer[TRACK_MODULE_SEGMENT_BUFSIZ];
    snprintf(buffer, TRACK_MODULE_SEGMENT_BUFSIZ, "{\"userId\": \"%s\", \"event\": \"%s\",\"properties\": %s}", u.text, e.text, data);
    
    zpl_file_close(&e.f);
    zpl_file_close(&u.f);
    return track__module_segment_send(TRACK_MODULE_SEGMENT_TRACK_API, h->write_key, buffer);
}

TRACK_IDENT_PROC(segment__ident_handler) {
    module_segment *h = (module_segment*)user_data;
    track_escaped_string u = track_escape_string(user_id, "\"", '\\');
    
    static char buffer[TRACK_MODULE_SEGMENT_BUFSIZ];
    snprintf(buffer, TRACK_MODULE_SEGMENT_BUFSIZ, "{\"userId\": \"%s\", \"traits\": %s}", u.text, traits);
    
    zpl_file_close(&u.f);
    return track__module_segment_send(TRACK_MODULE_SEGMENT_IDENTIFY_API, h->write_key, buffer);
}

TRACK_GROUP_PROC(segment__group_handler) {
    module_segment *h = (module_segment*)user_data;
    track_escaped_string u = track_escape_string(user_id, "\"", '\\');
    track_escaped_string g = track_escape_string(group_id, "\"", '\\');
    
    static char buffer[TRACK_MODULE_SEGMENT_BUFSIZ];
    snprintf(buffer, TRACK_MODULE_SEGMENT_BUFSIZ, "{\"userId\": \"%s\", \"groupId\": \"%s\", \"traits\": %s}", u.text, g.text, traits);
    
    zpl_file_close(&u.f);
    zpl_file_close(&g.f);
    return track__module_segment_send(TRACK_MODULE_SEGMENT_GROUP_API, h->write_key, buffer);
}

TRACK_MODULE_UNREGISTER_PROC(segment__unregister_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    module_segment *h = (module_segment*)user_data;
    zpl_mfree(h);
    return 0;
}

int track_module_segment_register(char const *write_key) {
    ZPL_ASSERT_NOT_NULL(write_key);
    module_segment *h = (module_segment*)zpl_malloc(sizeof(module_segment));
    *h = (module_segment){
        .write_key = write_key
    };
    return track_module_register(segment__event_handler, segment__ident_handler, segment__group_handler, segment__unregister_handler, (void*)h);
}
