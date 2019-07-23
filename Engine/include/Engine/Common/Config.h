#pragma once

/* ZHU_API is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
*/
#if defined(_WIN32) && defined(ZHU_BUILD_DLL)
#    define ZHU_API __declspec(dllexport)
#elif defined(_WIN32) && defined(ZHU_DLL)
#    define ZHU_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(ZHU_BUILD_DLL)
#    define ZHU_API __attribute__((visibility("default")))
#else // Build and use it as a static library
#define ZHU_API
#endif