#pragma once

#ifdef TRACK_SHARED_LIB
    #if defined(_WIN32) || defined(__CYGWIN__)
        #ifdef TRACK_EXPORTS
            #define TRACK_API __declspec(dllexport)
        #else
            #define TRACK_API __declspec(dllimport)
        #endif
    #else
        #ifdef TRACK_EXPORTS
            #define TRACK_API __attribute__((__visibility__("default")))
        #else
            #define TRACK_API __attribute__((__visibility__("hidden")))
        #endif
    #endif
#else
#define TRACK_API
#endif
