#pragma once

#include <numeric>
#include <limits>
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdint.h>
#include <assert.h>

#define NTGS_USE_DOUBLE_AS_FLOAT 0

#if NTGS_USE_DOUBLE_AS_FLOAT
    #define FLOAT double
#else
    #define FLOAT float
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__LP64__) || defined(_LP64) || defined(WIN64) 
    #define NTGS_64BITS
#else
    #define NTGS_32BITS
#endif

#define EPSILON std::numeric_limits<float>::epsilon()
#define MAXF std::numeric_limits<float>::max();
#define MINF std::numeric_limits<float>::min();

#define SIZEOF_EQ(a, b) assert(sizeof(a) == sizeof(b));
#define STATIC_SIZEOF_EQ(A, B) static_assert(sizeof(a) == sizeof(b), "STATIC_SIZEOF_EQ() assertion failed");

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