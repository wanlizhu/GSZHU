#pragma once

#ifdef _DEBUG
#define LOG_ENABLED 1
#else
#define LOG_ENABLED 0
#endif

#define PROFILING_ENABLED 1
#define PROFILING_LOG 0
#define PROFILING_LOG_BATCH_SIZE 1024

#define NVAPI_ENABLED 0
#define GS_USE_PYTHON 0
#define GS_USE_D3D12
//#define GS_USE_VULKAN
//#define GS_USE_OPENGL 


// Define DLL export/import
#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) 
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define DLL_IMPORT extern
#endif

#ifdef BUILD_SHARED_LIBS
#define GS_API     DLL_EXPORT
#define GS_DEV_API DLL_EXPORT
#elif BUILD_STATIC_LIBS
#define GS_API
#else
#define GS_API DLL_IMPORT
#endif


namespace GS
{
#define TO_ENUM(T, a) static_cast<T>(a)
#define TO_ULT(T, a)  static_cast<std::underlying_type<T>::type>(a)
#define ENUM_CLASS_OPERATORS(E)\
    inline E  operator& (E  a, E  b) { return TO_ENUM(E, TO_ULT(E, a) & TO_ULT(E, b)); }\
    inline E  operator| (E  a, E  b) { return TO_ENUM(E, TO_ULT(E, a) | TO_ULT(E, b)); }\
    inline E& operator&=(E& a, E& b) { a = a & b; return a; }\
    inline E& operator|=(E& a, E& b) { a = a | b; return a; }\
    inline E  operator~(E a) { return TO_ENUM(E, ~TO_ULT(E, a)); }\
    inline bool is_set(E val, E flag) { return (val & flag) != (E)0; }
}