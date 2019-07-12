#pragma once

#ifdef _MSC_VER
#define ZHU_DLL_EXPORT __declspec(dllexport)
#define ZHU_DLL_IMPORT __declspec(dllimport)
#else
#define ZHU_DLL_EXPORT __attribute__ ((visibility ("default")))
#define ZHU_DLL_IMPORT extern
#endif

#ifdef ZHU_STATIC
#    define ZHU_API
#else
#    ifdef ZHU_SHARED_EXPORTS
#        define ZHU_API ZHU_DLL_EXPORT
#    else
#        define ZHU_API ZHU_DLL_IMPORT
#    endif
#endif
