#pragma once

#ifdef _MSC_VER
#define ZHU_DLL_EXPORT __declspec(dllexport)
#define ZHU_DLL_IMPORT __declspec(dllimport)
#else
#define ZHU_DLL_EXPORT __attribute__ ((visibility ("default")))
#define ZHU_DLL_IMPORT extern
#endif

#ifdef ZHU_BUILD_SHARED
#    define ZHU_API ZHU_DLL_EXPORT
#elif defined(ZHU_BUILD_STATIC)
#    define ZHU_API 
#elif defined(ZHU_USE_SHARED)
#    define ZHU_API ZHU_DLL_IMPORT
#elif defined(ZHU_USE_STATIC)
#    define ZHU_API
#else
#    error "!!!No API export/import macro specified"
#endif
