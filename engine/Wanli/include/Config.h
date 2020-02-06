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

#ifdef WANLI_BUILD_SHARED_LIBS
#define DLLDECL WANLI_EXPORT
#else  
#define DLLDECL WANLI_IMPORT
#endif 

#define ENGINE_NAME "Wanli"