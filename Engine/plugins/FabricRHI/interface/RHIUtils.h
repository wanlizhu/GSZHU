#pragma once

#ifdef _WIN32
#define RHI_EXPORT __declspec(dllexport)
#define RHI_IMPORT __declspec(dllimport)
#else
#define RHI_EXPORT __attribute__((visibility("default")))
#define RHI_IMPORT 
#endif

#ifdef RHI_BUILD_SHARED_LIBS
#define RHI_API RHI_EXPORT
#elif defined RHI_STATIC
#define RHI_API
#else
#define RHI_API RHI_IMPORT
#endif

