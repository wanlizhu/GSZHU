#pragma once

#ifdef _MSVC_LANG
#define CXX_API_EXPORT __declspec(dllexport)
#define CXX_API_IMPORT __declspec(dllimport)
#else
#define CXX_API_EXPORT __attribute__((visibility("default")))
#define CXX_API_IMPORT
#endif

#ifdef GE_BUILD_SHARED_LIBS
#define GE2_IMPEXP CXX_API_EXPORT
#elif defined(GE_STATIC_LIBS)
#define GE2_IMPEXP
#else
#define GE2_IMPEXP CXX_API_IMPORT
#endif


