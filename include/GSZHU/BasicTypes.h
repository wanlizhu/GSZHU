#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <numeric>
#include <stdint.h>
#include <math.h>
#include <atomic>
#include <stdint.h>

namespace GSZHU {
#ifdef GSZHU_EXPORT_SHARED
#define GSZHU_API  __declspec(dllexport)
#elif defined(GSZHU_IMPORT_SHARED)
#define GSZHU_API __declspec(dllimport)
#else
#define GSZHU_API 
#endif

    typedef void* WINDOW_HANDLE;
    typedef std::string STRING;
    typedef std::wstring WSTRING;
    typedef std::vector<std::string> STRING_LIST;
    typedef std::unordered_map<std::string, std::string> STRING_MAP;
    
    template<typename T>
    using SHARED = std::shared_ptr<T>;

    template<typename T>
    using WEAK = std::weak_ptr<T>;

    template<typename K, typename V>
    using HASH_MAP = std::unordered_map<K, V>;

    template<typename T>
    using UNDERLYING_TYPE = typename std::underlying_type<T>::type;

    struct GSZHU_API RECT {
        int Left = 0;
        int Top = 0;
        int Right = 0;
        int Bottom = 0;
    };
                    
    struct GSZHU_API Box3D {
        int MinX = 0; 
        int MaxX = 0; 
        int MinY = 0; 
        int MaxY = 0; 
        int MinZ = 0; 
        int MaxZ = 1; 
    };


#define FLT32_EQ(a, b) (std::abs(a - b) <= std::numeric_limits<float>::epsilon())
#define FLT64_EQ(a, b) (std::abs(a - b) <= std::numeric_limits<double>::epsilon())
#define FLT_EQ FLT32_EQ

#define DEFINE_FLAG_ENUM_OPS(ENUM)\
extern "C++"\
{\
inline           ENUM& operator |= (ENUM& a, ENUM b) throw() { return reinterpret_cast<ENUM&>(reinterpret_cast<UNDERLYING_TYPE<ENUM>&>(a) |= static_cast<UNDERLYING_TYPE<ENUM>>(b)); } \
inline           ENUM& operator &= (ENUM& a, ENUM b) throw() { return reinterpret_cast<ENUM&>(reinterpret_cast<UNDERLYING_TYPE<ENUM>&>(a) &= static_cast<UNDERLYING_TYPE<ENUM>>(b)); } \
inline           ENUM& operator ^= (ENUM& a, ENUM b) throw() { return reinterpret_cast<ENUM&>(reinterpret_cast<UNDERLYING_TYPE<ENUM>&>(a) ^= static_cast<UNDERLYING_TYPE<ENUM>>(b)); } \
inline constexpr ENUM  operator |  (ENUM  a, ENUM b) throw() { return static_cast<ENUM> (static_cast<UNDERLYING_TYPE<ENUM>>(a) |  static_cast<UNDERLYING_TYPE<ENUM>>(b)); } \
inline constexpr ENUM  operator &  (ENUM  a, ENUM b) throw() { return static_cast<ENUM> (static_cast<UNDERLYING_TYPE<ENUM>>(a) &  static_cast<UNDERLYING_TYPE<ENUM>>(b)); } \
inline constexpr ENUM  operator ^  (ENUM  a, ENUM b) throw() { return static_cast<ENUM> (static_cast<UNDERLYING_TYPE<ENUM>>(a) ^  static_cast<UNDERLYING_TYPE<ENUM>>(b)); } \
inline constexpr ENUM  operator ~  (ENUM  a)         throw() { return static_cast<ENUM> (~static_cast<UNDERLYING_TYPE<ENUM>>(a)); } \
}

} 
