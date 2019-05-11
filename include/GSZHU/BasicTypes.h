#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <numeric>
#include <stdint.h >
#include <math.h>

namespace GSZHU {
    typedef void* WINDOW_HANDLE;
    typedef std::string STRING;
    typedef std::wstring WSTRING;
    typedef std::vector<std::string> STRING_LIST;
    typedef std::unordered_map<std::string, std::string> STRING_MAP;
    typedef uint8_t UINT8;
    typedef int8_t INT8;
    typedef uint16_t UINT16;
    typedef int16_t INT16;
    typedef uint32_t UINT;
    typedef int32_t INT;
    typedef uint64_t UINT64;
    typedef int64_t INT64;

    typedef std::array<int, 2> INT2;
    typedef std::array<int, 3> INT3;
    typedef std::array<int, 4> INT4;
    
    template<typename T>
    using SHARED = std::shared_ptr<T>;

    template<typename K, typename V>
    using HASH_MAP = std::unordered_map<K, V>;

    template<typename T>
    using UNDERLYING_TYPE = typename std::underlying_type<T>::type;

    struct RECT {
        INT Left = 0;
        INT Top = 0;
        INT Right = 0;
        INT Bottom = 0;
    };

    struct Box3D {
        UINT MinX = 0; 
        UINT MaxX = 0; 
        UINT MinY = 0; 
        UINT MaxY = 0; 
        UINT MinZ = 0; 
        UINT MaxZ = 1; 

        Box3D() {}
        Box3D(UINT _MinX, UINT _MaxX, UINT _MinY, UINT _MaxY, UINT _MinZ, UINT _MaxZ) 
            : MinX(_MinX), MaxX(_MaxX)
            , MinY(_MinY), MaxY(_MaxY)
            , MinZ(_MinZ), MaxZ(_MaxZ)
        {}

        Box3D(UINT _MinX, UINT _MaxX, UINT _MinY, UINT _MaxY) 
            : Box3D(_MinX, _MaxX, _MinY, _MaxY, 0, 1)
        {}

        Box3D(UINT _MinX, UINT _MaxX) 
            : Box3D(_MinX, _MaxX, 0, 0, 0, 1)
        {}
    };


#define FT32_EQUAL(a, b) (std::abs(a - b) <= std::numeric_limits<float>::epsilon())
#define FT64_EQUAL(a, b) (std::abs(a - b) <= std::numeric_limits<double>::epsilon())

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