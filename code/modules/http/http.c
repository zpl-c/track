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

typedef struct {
    CURL *curl;
    char const *url;
    track_module_http_payload_proc *proc;
} module_http;

zpl_thread_local int endpoints_intialised = 0;

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
        curl_easy_setopt(h->curl, CURLOPT_URL, h->url);
        curl_easy_setopt(h->curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(h->curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(h->curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(h->curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(h->curl, CURLOPT_POSTFIELDS, req);
        curl_easy_setopt(h->curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(h->curl, CURLOPT_USERAGENT, "libtrack/1.0.0");
        
        int res = curl_easy_perform(h->curl);
        
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
    curl_easy_cleanup(h->curl);
    
    if (--endpoints_intialised == 0) {
        curl_global_cleanup();
    }
    
    zpl_mfree(h);
    return 0;
}

int track_module_http_register(char const *url, track_module_http_payload_proc *proc) {
    ZPL_ASSERT_NOT_NULL(url);
    ZPL_ASSERT_NOT_NULL(proc);
    if (endpoints_intialised == 0) {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    endpoints_intialised++;
    
    module_http *h = (module_http*)zpl_malloc(sizeof(module_http));
    *h = (module_http){
        .curl = curl_easy_init(),
        .url = url,
        .proc = proc
    };
    
    if (!h->curl) {
        zpl_mfree(h);
        return -1;
    }
    return track_module_register(http__event_handler, http__unregister_handler, (void*)h);
}

void track_module_http_add_header(void **headers, char const *header) {
    *headers = curl_slist_append(*headers, header);
}