#pragma once

#include <numeric>
#include <limits>
#include <memory>
#include <stdint.h>
#include <assert.h>

#define NTGS_SSE_ENABLED 1

#if defined(__x86_64__) || defined(_M_X64) || defined(__LP64__) || defined(_LP64) || defined(WIN64) 
    #define NTGS_64BITS
#else
    #define NTGS_32BITS
#endif

#define EPSILON std::numeric_limits<float>::epsilon()
#define MAXF std::numeric_limits<float>::max();
#define MINF std::numeric_limits<float>::min();

#ifdef _WIN32
    #pragma warning(disable : 4251) // 'field' : class 'A' needs to have dll-interface to be used by clients of class 'B'
    #pragma warning(disable : 4800) // 'type' : forcing value to bool 'true' or 'false' (performance warning)
    #pragma warning(disable : 4996) // Secure SCL warnings

    #define NTGS_OS_WINDOWS

    #define _CRT_SECURE_NO_WARNINGS
    #define _CRT_NONSTDC_NO_DEPRECATE
    #define _CRT_SECURE_NO_DEPRECATE
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
#elif defined(__APPLE__)
    #define NTGS_OSX
#elif defined(__linux)
    #define NTGS_LINUX
#else
    #error Unknown OS
#endif

#ifdef _WIN32
    #define NTGS_EXPORT __declspec(dllexport)
    #define NTGS_IMPORT __declspec(dllimport)

    #define FINLINE __forceinline
    #define NOINLINE __declspec(noinline)
    #define ALIGN16 __declspec(align(16))
    #define ALIGN32 __declspec(align(32))
    #define ALIGN64 __declspec(align(64))
    #define EMPTY_BASES __declspec(empty_bases) 

    #include <stdint.h>
    #include <float.h>
    #include <xmmintrin.h>
    #define snprintf _snprintf
    #define vsnprintf _vsnprintf
    #ifdef NTGS_64BITS
        typedef long long ssize_t;
    #else
        typedef long ssize_t;
    #endif
#else
    #define NTGS_EXPORT __attribute__ ((visibility("default")))
    #define NTGS_IMPORT

    #define FINLINE inline __attribute__((always_inline))
    #define NOINLINE __attribute__((noinline))
    #define ALIGN16 __attribute__((aligned(16)))
    #define ALIGN32 __attribute__((aligned(32)))
    #define ALIGN64 __attribute__((aligned(64)))
    #define EMPTY_BASES
#endif

#ifdef NTGS_BUILD_SHARED
    #define NTGS_API NTGS_EXPORT
#else
    #define NTGS_API 
#endif

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT;
typedef uint32_t UINT;
typedef int64_t INT64;
typedef uint64_t UINT64;
typedef size_t SIZE;
typedef ssize_t SSIZE;
typedef float FLOAT;
typedef double DOUBLE;
