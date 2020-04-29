#pragma once

#include "config.h"

#if defined(_MSC_VER)
#    define DJV_EXPORT   __declspec(dllexport)
#    define DJV_IMPORT   __declspec(dllimport)
#    define DJV_NOINLINE __declspec(noinline)
#    define DJV_INLINE   __forceinline
#else
#    define DJV_EXPORT    __attribute__ ((visibility("default")))
#    define DJV_IMPORT
#    define DJV_NOINLINE  __attribute__ ((noinline))
#    define DJV_INLINE    __attribute__((always_inline)) inline
#endif

#if defined(DJV_BUILD_SHARED_LIBS)
#   define DJV_API DJV_EXPORT
#else
#   define DJV_API DJV_IMPORT
#endif

#if defined(DJV_STATIC_LIBS)
#   if defined(DJV_API)
#       undef DJV_API
#   endif
#   define DJV_API
#endif

#define DJV_EXPAND(x) x
#define DJV_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, COUNT, ...)  COUNT
#define DJV_VA_SIZE(...) DJV_EXPAND(DJV_COUNT(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1))

/* Reduce namespace pollution from windows.h */
#if defined(_WIN32)
#    if !defined(WIN32_LEAN_AND_MEAN)
#        define WIN32_LEAN_AND_MEAN
#    endif
#    if !defined(NOMINMAX)
#        define NOMINMAX
#    endif
#    if !defined(_USE_MATH_DEFINES)
#        define _USE_MATH_DEFINES
#    endif
#endif

/* Processor architecture */
#if defined(_MSC_VER)
#    if defined(_M_X86) && !defined(__i386__)
#        error 32-bit builds are not supported. Please run cmake-gui.exe, delete the cache, and \
               regenerate a new version of the build system that uses a 64 bit version of the compiler
#    endif
#    if defined(_M_X64) && !defined(__x86_64__)
#        define __x86_64__
#    endif
#endif

#if defined(_MSC_VER) // warning C4127: conditional expression is constant
#    pragma warning (disable: 4127)
#endif