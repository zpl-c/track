/**
  track - C99 event tracking library

Usage:
#  define TRACK_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

#  define TRACK_IMPLEMENTATION
#  include "track.h"

Options:

GitHub:
  https://github.com/zpl-c/track

Version History:
  @{CHANGELOG}

License:
  @{LICENSE}
*/

#ifndef TRACK_H
#define TRACK_H

#define TRACK_VERSION_MAJOR 2
#define TRACK_VERSION_MINOR 1
#define TRACK_VERSION_PATCH 0
#define TRACK_VERSION_PRE ""

#include "zpl_hedley.h"

#define TRACK_VERSION TRACK_VERSION_ENCODE(TRACK_VERSION_MAJOR, TRACK_VERSION_MINOR, TRACK_VERSION_PATCH)

#ifdef TRACK_IMPL
#    ifndef TRACK_IMPLEMENTATION
#    define TRACK_IMPLEMENTATION
#    endif
#endif

#if defined(__cplusplus) && !defined(TRACK_EXTERN)
#    define TRACK_EXTERN extern "C"
#else
#    define TRACK_EXTERN extern
#endif

#ifndef TRACK_DEF
#    if defined(TRACK_SHARED_LIB)
#        ifdef TRACK_IMPLEMENTATION
#            define TRACK_DEF TRACK_PUBLIC
#        else
#            define TRACK_DEF TRACK_IMPORT
#        endif
#    elif defined(TRACK_STATIC_LIB)
#        ifdef TRACK_IMPLEMENTATION
#            define TRACK_DEF
#        else
#            define TRACK_DEF TRACK_EXTERN
#        endif
#    elif defined(TRACK_STATIC)
#        define TRACK_DEF static
#    else
#        define TRACK_DEF TRACK_EXTERN
#    endif
#endif

#ifndef TRACK_DEF_INLINE
#    if defined(TRACK_STATIC)
#        define TRACK_DEF_INLINE
#        define TRACK_IMPL_INLINE
#    else
#        define TRACK_DEF_INLINE static
#        define TRACK_IMPL_INLINE static inline
#    endif
#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-function"
#    pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#    pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4201)
#    pragma warning(disable : 4127) // Conditional expression is constant
#endif

/* general purpose includes */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#if defined(TRACK_SYSTEM_WINDOWS)
#    include <intrin.h>
#endif

#include "header/core.h"

#if defined(TRACK_COMPILER_MSVC)
#    pragma warning(pop)
#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
#    pragma GCC diagnostic pop
#endif

#if defined(TRACK_IMPLEMENTATION) && !defined(TRACK_IMPLEMENTATION_DONE)
#define TRACK_IMPLEMENTATION_DONE

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wattributes"
#    pragma GCC diagnostic ignored "-Wunused-value"
#    pragma GCC diagnostic ignored "-Wunused-function"
#    pragma GCC diagnostic ignored "-Wwrite-strings"
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#    pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#    pragma GCC diagnostic ignored "-Wmissing-braces"
#    pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#    pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#    pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4201)
#    pragma warning(disable : 4996) // Disable deprecated POSIX functions warning
#    pragma warning(disable : 4127) // Conditional expression is constant
#    pragma warning(disable : 4204) // non-constant field initializer
#    pragma warning(disable : 4756) // -INFINITY
#    pragma warning(disable : 4056) // -INFINITY
#    pragma warning(disable : 4702) // unreachable code
#endif

/* general purpose includes */

#include <stdio.h>
#include <errno.h>

#if defined(TRACK_SYSTEM_UNIX) || defined(TRACK_SYSTEM_MACOS)
#    include <unistd.h>
#elif defined(TRACK_SYSTEM_WINDOWS)
#    if !defined(TRACK_NO_WINDOWS_H)
#        ifndef WIN32_LEAN_AND_MEAN
#            ifndef NOMINMAX
#            define NOMINMAX
#            endif

#            define WIN32_LEAN_AND_MEAN
#            define WIN32_MEAN_AND_LEAN
#            define VC_EXTRALEAN
#        endif
#        include <windows.h>
#        undef NOMINMAX
#        undef WIN32_LEAN_AND_MEAN
#        undef WIN32_MEAN_AND_LEAN
#        undef VC_EXTRALEAN
#    endif
#endif

#define SWRAP_IMPLEMENTATION
#include "swrap.h"

#include "source/core.c"

#if defined(TRACK_COMPILER_MSVC)
#    pragma warning(pop)
#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
#    pragma GCC diagnostic pop
#endif

#endif // TRACK_IMPLEMENTATION
#endif // TRACK_H

//TOC:
//track.h
//zpl_hedley.h
//header/core.h
//swrap.c
//source/core.c
