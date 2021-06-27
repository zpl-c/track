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
    char const *url;
    track_module_http_payload_proc *proc;
} module_http;

TRACK_EVENT_PROC(http__event_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    int ret_code = 0;
    track_escaped_string e = track_escape_string(event_id, "\"", '\\');
    track_escaped_string u = track_escape_string(user_id, "\"", '\\');
    track_escaped_string t = track_escape_string(data, "\"", '\\');
    
    module_http *h = (module_http*)user_data;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
    
    char *req = (char const *)h->proc(e.text, u.text, t.text, (void**)&headers);
    
    if (req) {
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, h->url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, req);
        
        int res = track_curl_send_message(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "track_module_http curl error: %s\n", curl_easy_strerror(res));
            ret_code = -1;
        }
        
        curl_slist_free_all(headers);
        free(req);
    } else {
        ret_code = -2;
    }
    
    zpl_file_close(&e.f);
    zpl_file_close(&u.f);
    zpl_file_close(&t.f);
    return ret_code;
}

TRACK_MODULE_UNREGISTER_PROC(http__unregister_handler) {
    ZPL_ASSERT_NOT_NULL(user_data);
    module_http *h = (module_http*)user_data;
    zpl_mfree(h);
    return 0;
}

int track_module_http_register(char const *url, track_module_http_payload_proc *proc) {
    ZPL_ASSERT_NOT_NULL(url);
    ZPL_ASSERT_NOT_NULL(proc);
    module_http *h = (module_http*)zpl_malloc(sizeof(module_http));
    *h = (module_http){
        .url = url,
        .proc = proc
    };
    return track_module_register(http__event_handler, http__unregister_handler, (void*)h);
}

void track_module_http_add_header(void **headers, char const *header) {
    *headers = curl_slist_append(*headers, header);
}