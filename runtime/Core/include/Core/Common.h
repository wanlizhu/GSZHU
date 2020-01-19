#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
#include <array>
#include <algorithm>
#include <map>
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

#ifdef _DEBUG
#define AssertionFailed(msg, ...) \
do { \
    LogFatal(msg, ##__VA_ARGS__); \
} while(false)

#define Unexpected(msg, ...) AssertionFailed(msg, ##__VA_ARGS__)

#define CheckError(expr, msg, ...) \
do { \
    if (!(expr)) { AssertionFailed(msg, ##__VA_ARGS__); } \
} while (false)
#else
#define AssertionFailed(...) do{} while(false)
#define Unexpected(msg, ...) do{} while(false)
#define CheckError(...) do{} while(false)
#endif

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
    using Handle = void*;

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
    using Array = std::shared_ptr<std::vector<_Value_>>;
    using ByteArray = Array<Uint8>;
    using Blob = ByteArray;
    using StringArray = Array<String>;

    template<typename _Value_, typename... _Args_>
    inline Array<_Value_> MakeArray(_Args_&&... args) 
    {
        return std::make_shared<std::vector<_Value_>>(std::forward<ARGS>(args)...);
    }

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

    enum class EKey : Uint
    {
        // ASCII values. Do not change them.
        Space = ' ',
        Apostrophe = '\'',
        Comma = ',',
        Minus = '-',
        Period = '.',
        Slash = '/',
        Key0 = '0',
        Key1 = '1',
        Key2 = '2',
        Key3 = '3',
        Key4 = '4',
        Key5 = '5',
        Key6 = '6',
        Key7 = '7',
        Key8 = '8',
        Key9 = '9',
        Semicolon = ';',
        Equal = '=',
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',
        LeftBracket = '[',
        Backslash = '\\',
        RightBracket = ']',
        GraveAccent = '`',

        // Special keys
        Escape,
        Tab,
        Enter,
        Backspace,
        Insert,
        Del,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDel,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper, // Windows key on windows
        RightShift,
        RightControl,
        RightAlt,
        RightSuper, // Windows key on windows
        Menu,
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

    template<typename T>
    inline Bool ArrayEquals(const T* a, const T* b, size_t N)
    {
        if (a == nullptr && b == nullptr) {
            return true;
        }   
        for (int i = 0; i < N; i++) {
            if (!(a[i] == b[i])) {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename U>
    inline std::shared_ptr<T> ToPtr(std::shared_ptr<U> ptr)
    {
        static_assert(std::is_base_of_v<U, T>);
        return std::dynamic_pointer_cast<T>(ptr);
    }

    template<typename T>
    inline String ToString(T val);

    struct Range
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint offset = 0;
        Uint size = kMax;

        static const Range& All()
        {
            static const Range all(0, kMax);
            return all;
        }

        Range() = default;
        Range(Uint offset, Uint size = kMax)
            : offset(offset)
            , size(size)
        {}

        inline bool IsEmpty() const
        {
            return size == 0;
        }

        inline bool IsAll() const
        {
            return *this == All();
        }

        inline bool operator==(const Range& rhs) const
        {
            return offset == rhs.offset && size == rhs.size;
        }
    };

    struct Extent
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint width = kMax;
        Uint height = kMax;

        static const Extent& All()
        {
            static const Extent all(kMax, kMax);
            return all;
        }

        Extent() = default;
        Extent(Uint w, Uint h)
            : width(w)
            , height(h)
        {}

        inline bool IsEmpty() const
        {
            return width == 0 && height == 0;
        }

        inline bool IsAll() const
        {
            return *this == All();
        }

        inline bool operator==(const Extent& rhs) const
        {
            return width == rhs.width && height == rhs.height;
        }
    };

    struct Offset
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint x = 0;
        Uint y = 0;

        static const Offset& Zero()
        {
            static const Offset zero(0, 0);
            return zero;
        }

        Offset() = default;
        Offset(Uint x, Uint y)
            : x(x)
            , y(y)
        {}

        inline bool IsZero() const
        {
            return *this == Zero();
        }

        inline bool operator==(const Offset& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        inline Offset operator+(const Extent& extent) const
        {
            return Offset(x + extent.width, y + extent.height);
        }

        inline Offset& operator+=(const Extent& extent)
        {
            *this = *this + extent;
            return *this;
        }
    };

    struct Region
    {
        Offset offset;
        Extent extent;

        Region() = default;
        Region(const Offset& offset, const Extent& extent)
            : offset(offset)
            , extent(extent)
        {}

        inline bool IsEmpty() const
        {
            return extent.IsEmpty();
        }

        inline bool IsAll() const
        {
            return offset.IsZero() && extent.IsAll();
        }

        inline bool operator==(const Region& rhs) const
        {
            return offset == offset && extent == extent;
        }
    };

    class AsyncData
    {
    public:

    private:

    };
}
