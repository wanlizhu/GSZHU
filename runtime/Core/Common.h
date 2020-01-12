#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <codecvt>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <any>
#include <tuple>
#include <variant>
#include <thread>
#include <mutex>
#include <optional>
#include <condition_variable>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Config.h"
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


#define ArraySize(a) (sizeof(a) / sizeof(a[0]))
#define OffsetOf(T, m) (size_t)((ptrdiff_t)&reinterpret_cast<const volatile char&>((((T*)0)->m)))
#define SafeDelete(a) { if (a != nullptr) { delete a; a = nullptr; } }
#define SafeDeleteArray(a) { if (a != nullptr) { delete[] a; a = nullptr; } }
#define Stringize(a) #a
#define ConcatStrings(a, b) a##b
#define EnumToInt(T, var) static_cast<std::underlying_type_t<T>>(var)
#define StringEqual(a, b) (strcmp(a, b) == 0)
#define StringEqual2(a, b1, b2) (strcmp(a, b1) == 0 || strcmp(a, b2) == 0)
#define StringEqual3(a, b1, b2, b3) (strcmp(a, b1) == 0 || strcmp(a, b2) == 0 || strcmp(a, b3) == 0)

#define ENUM_CLASS_OPERATORS(T) \
inline constexpr T  operator& (T  a, T b) { return static_cast<T>(EnumToInt(T, a) & EnumToInt(T, b)); } \
inline constexpr T  operator| (T  a, T b) { return static_cast<T>(EnumToInt(T, a) | EnumToInt(T, b)); } \
inline constexpr T& operator|=(T& a, T b) { a = a | b; return a; }; \
inline constexpr T& operator&=(T& a, T b) { a = a & b; return a; }; \
inline constexpr T  operator~ (T a) { return static_cast<T>(~EnumToInt(T, a)); } \
inline constexpr bool HasBits(T val, T flag) { return (val & flag) != (T)0; }


#define CLASSINFO(type) \
private: \
    using __class = type;\
    static constexpr const char* __className = #type;


#define FUNCTION_AND_LINE (std::string(__FUNCTION__) + ":" + std::to_string(__LINE__)).c_str()


#define DEFINE_BIT_OPS(type, target) \
inline bool Has(type bit) const { return target & bit; }\
inline void Set(type bit) { target |= bit; }\
inline void Unset(type bit) { target &= ~bit; }\
inline void Toggle(type bit) { target = Has(bit) ? (target & ~bit) : (target | bit); }


#define DEFINE_CMP_OPS(type, member) \
inline bool operator==(const type& rhs) const { return member == rhs.member; }\
inline bool operator!=(const type& rhs) const { return member != rhs.member; }\
inline bool operator< (const type& rhs) const { return member <  rhs.member; }\
inline bool operator<=(const type& rhs) const { return member <= rhs.member; }\
inline bool operator> (const type& rhs) const { return member >  rhs.member; }\
inline bool operator>=(const type& rhs) const { return member >= rhs.member; }


#define DEFINE_VEC_NUMERIC_OPS(type, N, data) \
inline type  operator+(const type& rhs) const { return type(*this) += rhs; }\
inline type  operator-(const type& rhs) const { return type(*this) -= rhs; }\
inline type  operator*(const type& rhs) const { return type(*this) *= rhs; }\
inline type  operator/(const type& rhs) const { return type(*this) /= rhs; }\
inline type& operator+=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, +=); return *this; }\
inline type& operator-=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, -=); return *this; }\
inline type& operator*=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, *=); return *this; }\
inline type& operator/=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, /=); return *this; }\
inline type  operator-() const { type tmp; LOOP_OP(tmp.data, (*this).data, 0, N, =-); return tmp; }


#define INHERIT_SHARED_FROM_THIS(basetype) \
public: \
std::shared_ptr<__class> shared_from_this()\
{\
    basetype* base = static_cast<basetype*>(this);\
    std::shared_ptr<basetype> shared = base->shared_from_this();\
    return std::static_pointer_cast<__class>(shared);\
}\
std::shared_ptr<const __class> shared_from_this() const\
{\
    const basetype* base = static_cast<const basetype*>(this);\
    std::shared_ptr<const basetype> shared = base->shared_from_this();\
    return std::static_pointer_cast<const __class>(shared);\
}


namespace Wanlix
{
#ifdef _WIN32
    using WindowHandle = HWND;
    using DllHandle = HMODULE;
#else
    using WindowHandle = void*;
    using DllHandle = void*;
#endif

    using Int8 = int8_t;
    using Int16 = int16_t;
    using Int = int32_t;
    using Int32 = int32_t;
    using Int64 = int64_t;
    using Bool = bool;
    using Byte = uint8_t;
    using Uint8 = uint8_t;
    using Uint16 = uint16_t;
    using Uint = uint32_t;
    using Uint32 = uint32_t;
    using Uint64 = uint64_t;
    using Size = size_t;
    using Pvoid = void*;
    using Wstring = std::wstring;
    using String = std::string;
    using Path = std::wstring;
    
    template<typename _Value_>
    using StringMap = std::unordered_map<std::string, _Value_>;
    
    template<typename _Value_>
    using Array = std::vector<_Value_>;
    using ByteArray = std::vector<uint8_t>;
    using ByteArrayPtr = std::shared_ptr<ByteArray>;
    using StringArray = std::vector<std::string>;
    using StringArrayPtr = std::shared_ptr<StringArray>;

    template<typename _Key_, typename _Value_>
    using HashMap = std::unordered_map<_Key_, _Value_>;
    
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;
    
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T>
    using Optional = std::optional<T>;

    using Int2 = glm::ivec2;
    using Int3 = glm::ivec3;
    using Int4 = glm::ivec4;
    using Uint2 = glm::uvec2;
    using Uint3 = glm::uvec3;
    using Uint4 = glm::uvec4;
    using Float2 = glm::vec2;
    using Float3 = glm::vec3;
    using Float4 = glm::vec4;
    using Quaternion = glm::fquat;
    using Float2x2 = glm::mat2;
    using Float3x3 = glm::mat3;
    using Float4x4 = glm::mat4;


    enum class EMessageBoxButton
    {
        None,
        Ok,
        Cancel,
        Retry,
        Abort,
        Ignore,
    };

    enum class EMessageBoxType
    {
        Ok,
        OkCancel,
        RetryCancel,
        AbortRetryIgnore,
    };

    enum class EFileEvent
    {
        Changed,
        Created,
        Deleted,
        Renamed,
    };

    enum class EDebugSeverity
    {
        Info = 0,
        Warning,
        Error,
        FatalError,
    };

    struct DataBlockFlags
    {
        enum {
            None = 0,
            IsArray = 1,
            IsOwner = (1 << 1),
        };
    };

    template<typename T>
    inline Bool IsPowerOf2(T num) {
        Uint64 tmp = (Ui64)num;
        return (tmp & (tmp - 1)) == 0;
    }

    template<typename T>
    inline Bool Equals(T a, T b, T eps = T(0)) {
        if constexpr (std::is_floating_point_v<T>) {
            if (eps == T(0))
                eps = std::numeric_limits<T>::epsilon() * 100;
            return std::abs(a - b) < eps;
        }
        else {
            return a == b;
        }
    }

    struct Offset
    {
        Uint x = 0;
        Uint y = 0;
        Uint z = 0;
    };

    struct Extent
    {
        Uint width = 0;
        Uint height = 0;
        Uint depth = 1;
    };

    struct Region
    {
        Offset offset;
        Extent extent;
    };
}
