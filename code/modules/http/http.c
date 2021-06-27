#include "http.h"
#include "track.h"
#include <stdio.h>
#include <stdlib.h>

#define ZPL_NANO
#define ZPL_ENABLE_PARSER
#include "zpl.h"

#define CURL_STATICLIB
#include "curl/curl.h"

#include "track_utils.h"
#include "track_curl.h"

typedef struct {
    track_module_http_event_proc *proc;
} module_http;

int track_module_http_send(char const *url, char const *post, void **headers) {
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, *headers);
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post);
    
    int res = track_curl_send_message(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "track_module_http curl error: %s\n", curl_easy_strerror(res));
        return -1;
    }
    
    return 0;
}

TRACK_EVENT_PROC(http__event_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    track_escaped_string e = track_escape_string(event_id, "\"", '\\');
    track_escaped_string u = track_escape_string(user_id, "\"", '\\');
    track_escaped_string t = track_escape_string(data, "\"", '\\');
    
    module_http *h = (module_http*)user_data;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
    int err = h->proc(TRACK_HTTP_MSGKIND_TRACK, e.text, u.text, t.text, data, (void**)&headers);
    
    zpl_file_close(&e.f);
    zpl_file_close(&u.f);
    zpl_file_close(&t.f);
    return err;
}

TRACK_IDENT_PROC(http__ident_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    track_escaped_string u = track_escape_string(user_id, "\"", '\\');
    track_escaped_string t = track_escape_string(traits, "\"", '\\');
    
    module_http *h = (module_http*)user_data;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
    int err = h->proc(TRACK_HTTP_MSGKIND_IDENTIFY, "Identify", u.text, t.text, traits, (void**)&headers);
    
    zpl_file_close(&u.f);
    zpl_file_close(&t.f);
    return err;
}

TRACK_MODULE_UNREGISTER_PROC(http__unregister_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    module_http *h = (module_http*)user_data;
    zpl_mfree(h);
    return 0;
}

int track_module_http_register(track_module_http_event_proc *proc) {
    ZPL_ASSERT_NOT_NULL(proc);
    module_http *h = (module_http*)zpl_malloc(sizeof(module_http));
    *h = (module_http){
        .proc = proc
    };
    return track_module_register(http__event_handler, http__ident_handler, http__unregister_handler, (void*)h);
}

void track_module_http_add_header(void **headers, char const *header) {
    *headers = curl_slist_append(*headers, header);
}