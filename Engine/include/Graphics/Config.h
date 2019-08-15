#pragma once

#include <type_traits>

#if defined(_WIN32) && defined(ZHU_BUILD_DLL)
#    define ZHU_GRAPHICS_API __declspec(dllexport)
#elif defined(_WIN32) && defined(ZHU_USE_STATIC)
#    define ZHU_GRAPHICS_API
#elif defined(__GNUC__) && defined(ZHU_BUILD_DLL)
#    define ZHU_GRAPHICS_API __attribute__((visibility("default")))
#else // Build and use it as a static library
#    define ZHU_GRAPHICS_API __declspec(dllimport)
#endif

namespace ZHU
{
template<typename T>
using UnderType = typename std::underlying_type<T>::type;

#define DEFINE_ENUM_OPERATORS(E)\
extern "C++" {\
inline           E& operator |= (E& a, E& b) noexcept { return reinterpret_cast<E&>(reinterpret_cast<UnderType<E>&>(a) |= static_cast<UnderType<E>>(b)); }\
inline           E& operator &= (E& a, E& b) noexcept { return reinterpret_cast<E&>(reinterpret_cast<UnderType<E>&>(a) &= static_cast<UnderType<E>>(b)); }\
inline           E& operator ^= (E& a, E& b) noexcept { return reinterpret_cast<E&>(reinterpret_cast<UnderType<E>&>(a) ^= static_cast<UnderType<E>>(b)); }\
inline constexpr E  operator |  (E  a, E  b) noexcept { return static_cast<E>(static_cast<UnderType<E>>(a) | static_cast<UnderType<E>>(b)); }\
inline constexpr E  operator &  (E  a, E  b) noexcept { return static_cast<E>(static_cast<UnderType<E>>(a) & static_cast<UnderType<E>>(b)); }\
inline constexpr E  operator ^  (E  a, E  b) noexcept { return static_cast<E>(static_cast<UnderType<E>>(a) ^ static_cast<UnderType<E>>(b)); }\
inline constexpr E  operator ~  (E  a)       noexcept { return static_cast<E>(~static_cast<UnderType<E>>(a)); }\
}

}