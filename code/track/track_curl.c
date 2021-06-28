#include "track_curl.h"
#include <stdio.h>
#include <stdlib.h>

#define ZPL_NANO
#define ZPL_ENABLE_PARSER
#define ZPL_ENABLE_HASHING
#include "zpl.h"

#ifndef TRACK_DISABLE_CURL_SUPPORT
#include <curl/curl.h>


static CURLM *curlm_handle = 0;

int track_curl_init(int is_async) {
    curl_global_init(CURL_GLOBAL_ALL);
    if (is_async)
        curlm_handle = curl_multi_init();
    else
        curlm_handle = 0;
    return 0;
}

int track_curl_destroy(void) {
    if (curlm_handle) {
        curl_multi_cleanup(curlm_handle);
        curlm_handle = 0;
    }
    
    curl_global_cleanup();
    return 0;
}

int track_curl_send_message(void *handle) {
    ZPL_ASSERT_NOT_NULL(handle);
    int err;
    
    CURL *curl = (CURL*)handle;
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libtrack/1.0.0");
    
    if (curlm_handle)
        err = curl_multi_add_handle(curlm_handle, curl);
    else {
        err = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    return err;
}

int track_curl_poll(void) {
    if (!curlm_handle) return 0;
    int running_threads;
    CURLMcode code;
    
    code = curl_multi_perform(curlm_handle, &running_threads);
    
    if (code != CURLE_OK) {
        fprintf(stderr, "track curl error: %s\n", curl_easy_strerror(code));
    }
    
    CURLMsg *m = 0;
    do {
        int msgq = 0;
        m = curl_multi_info_read(curlm_handle, &msgq);
        if(m && (m->msg == CURLMSG_DONE)) {
            CURL *e = m->easy_handle;
            // TODO(zaklaus): find a way to free headers
            curl_multi_remove_handle(curlm_handle, e);
            curl_easy_cleanup(e);
        }
    } while(m);
    
    return running_threads;
}

int track_curl_perform(void) {
    int remaining_jobs = 0;
    do {
        remaining_jobs = track_curl_poll();
    } while (remaining_jobs > 0);
    
    return 0;
}
#else
int track_curl_init(int is_async) {
    return -1;
}

int track_curl_destroy(void) {
    return -1;
}

int track_curl_send_message(void *handle) {
    return -1;
}

int track_curl_poll(void) {
    return -1;
}

int track_curl_perform(void) {
    return -1;
}
#endif
