#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#ifdef _MSC_VER
#define WANLI_EXPORT __declspec(dllexport)
#define WANLI_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) 
#define WANLI_EXPORT __attribute__ ((visibility ("default")))
#define WANLI_IMPORT extern
#endif

#ifdef WANLI_STATIC_LIB
#   define DLLDECL
#else
#   ifdef WANLI_BUILD_SHARED_LIB
#       define WANLI_API WANLI_EXPORT
#   else
#       define WANLI_API WANLI_IMPORT
#   endif 
#endif

#ifdef PLATFORM_WINDOWS
#include "Windows/WindowsConfig.h"
#elif defined(PLATFORM_ANDROID)
#include "Windows/AndroidConfig.h"
#endif

#ifdef ENABLE_NVAPI
#include "nvapi.h"
#pragma comment(lib, "nvapi64.lib")
#endif
