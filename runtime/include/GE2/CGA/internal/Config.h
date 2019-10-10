#pragma once

#ifdef _MSVC_LANG
#define CXX_API_EXPORT __declspec(dllexport)
#define CXX_API_IMPORT __declspec(dllimport)
#else
#define CXX_API_EXPORT __attribute__((visibility("default")))
#define CXX_API_IMPORT
#endif

#ifdef GE_BUILD_SHARED_LIBS
#define CGA_API CXX_API_EXPORT
#elif defined(GE_STATIC_LIBS)
#define CGA_API
#else
#define CGA_API CXX_API_IMPORT
#endif

#ifndef CGA_API
#define CGA_API
#endif