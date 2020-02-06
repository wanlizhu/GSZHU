#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <exception>
#include <stdexcept>
#include <assert.h>

#define VK_CPU_ALLOCATOR nullptr

namespace Wanli
{
    template<typename... _Args_>
    String StringFormat(const char* fmt, _Args_... args)
    {
        static char buffer[1024];
        snprintf(buffer, 1024, fmt, std::forward<_Args_>(args)...);
        return buffer;
    }

    class DLLDECL RHIError : public std::runtime_error
    {
    public:
        RHIError(const String& msg)
            : std::runtime_error(msg)
        {}

        RHIError(const char* msg)
            : std::runtime_error(msg)
        {}

        template<typename... _Args_>
        RHIError(const char* fmt, _Args_... args)
            : std::runtime_error(StringFormat(fmt, std::forward<_Args_>(args)...).c_str())
        {}
    };
}