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
    typedef void* WINDOW_HANDLE;
    typedef std::string STRING;
    typedef std::wstring WSTRING;
    typedef std::vector<std::string> STRING_LIST;
    typedef std::unordered_map<std::string, std::string> STRING_MAP;

    typedef std::array<int, 2> INT2;
    typedef std::array<int, 3> INT3;
    typedef std::array<int, 4> INT4;
    
    template<typename T>
    using SHARED = std::shared_ptr<T>;

    template<typename T>
    using WEAK = std::weak_ptr<T>;

    template<typename K, typename V>
    using HASH_MAP = std::unordered_map<K, V>;

    template<typename T>
    using UNDERLYING_TYPE = typename std::underlying_type<T>::type;

    struct RECT {
        int Left = 0;
        int Top = 0;
        int Right = 0;
        int Bottom = 0;
    };
                    
    struct Box3D {
        uint32_t MinX = 0; 
        uint32_t MaxX = 0; 
        uint32_t MinY = 0; 
        uint32_t MaxY = 0; 
        uint32_t MinZ = 0; 
        uint32_t MaxZ = 1; 

        Box3D() {}
        Box3D(uint32_t _MinX, uint32_t _MaxX, uint32_t _MinY, uint32_t _MaxY, uint32_t _MinZ, uint32_t _MaxZ) 
            : MinX(_MinX), MaxX(_MaxX)
            , MinY(_MinY), MaxY(_MaxY)
            , MinZ(_MinZ), MaxZ(_MaxZ)
        {}

        Box3D(uint32_t _MinX, uint32_t _MaxX, uint32_t _MinY, uint32_t _MaxY) 
            : Box3D(_MinX, _MaxX, _MinY, _MaxY, 0, 1)
        {}

        Box3D(uint32_t _MinX, uint32_t _MaxX) 
            : Box3D(_MinX, _MaxX, 0, 0, 0, 1)
        {}
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

} // namespace GSZHU