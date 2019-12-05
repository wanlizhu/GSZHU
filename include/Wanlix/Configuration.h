#pragma once

#if defined _WIN32
#    define WANLIX_EXPORT __declspec(dllexport)
#    define WANLIX_IMPORT __declspec(dllimport)
#else
#    define WANLIX_EXPORT 
#    define WANLIX_IMPORT 
#endif

#if defined WANLIX_STATIC_LIBS
#    define WANLIX_API
#else
#    if defined WANLIX_BUILD_SHARED_LIBS
#        define WANLIX_API WANLIX_EXPORT
#    else
#        define WANLIX_API WANLIX_IMPORT
#    endif
#endif

