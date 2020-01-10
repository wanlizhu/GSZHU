#pragma once

#include <assert.h>

#if defined _WIN32
#   define WANLIX_OS_WINDOWS
#elif defined __APPLE__
#   include <TargetConditionals.h>
#   if TARGET_OS_OSX != 0
#       define WANLIX_OS_MACOS
#   elif TARGET_OS_IOS != 0
#       define WANLIX_OS_IOS
#   endif
#elif defined __linux__
#   define WANLIX_OS_LINUX
#elif defined __ANDROID__
#   define WANLIX_OS_ANDROID
#endif


#if defined _M_ARM || defined __arm__
#   define WANLIX_ARCH_ARM
#elif defined _M_X64 || defined __amd64__
#   define WANLIX_ARCH_AMD64
#elif defined _M_IX86 || defined _X86_ || defined __X86__ || defined __i386__
#   define WANLIX_ARCH_IA32
#endif

#if defined _WIN32
#   define DLLEXPORT __declspec(dllexport)
#   define DLLIMPORT __declspec(dllimport)
#else
#   define DLLEXPORT __attribute__((visibility("default")))
#   define DLLIMPORT __attribute__((visibility("default")))
#endif

#if defined _WIN32 && !defined WANLIX_STATIC
#    if defined WANLIX_BUILD_SHARED_LIBS
#        define DLL_DECL DLLEXPORT
#    else
#        define DLL_DECL DLLIMPORT
#    endif
#else
#    define WANLIX_API
#endif

#ifdef _WIN32
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif

    // identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#   pragma warning(disable: 4251)
    // non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#   pragma warning(disable: 4275)
    // no definition for inline function 'function'
#   pragma warning(disable: 4506)
#   pragma warning(disable: 4996)

#   define STDCALL     __stdcall
#   define FORCEINLINE __forceinline
#   define NOINLINE    __declspec(noinline)
#else // NOT Windows
#   define STDCALL
#   define FORCEINLINE __attribute__((always_inline))
#   define NOINLINE    __attribute__((noinline))
#endif
