#pragma once

#include <array>

#if defined(_WIN32) && defined(ZHU_BUILD_DLL)
#    define ZHU_COMMON_API __declspec(dllexport)
#elif defined(_WIN32) && defined(ZHU_USE_STATIC)
#    define ZHU_COMMON_API
#elif defined(__GNUC__) && defined(ZHU_BUILD_DLL)
#    define ZHU_COMMON_API __attribute__((visibility("default")))
#else // Build and use it as a static library
#    define ZHU_COMMON_API __declspec(dllimport)
#endif

namespace ZHU
{
    using  PCCH = const char*;
    using PCCHW = const wchar_t*;
    using  INT2 = std::array<int, 2>;
    using  INT3 = std::array<int, 3>;
    using  INT4 = std::array<int, 4>;
}