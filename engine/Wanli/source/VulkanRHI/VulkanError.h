#pragma once

#include "VulkanConfig.h"

namespace Wanli
{
    template<typename... _Args_>
    String StringFormat(const char* fmt, _Args_... args)
    {
        static char buffer[1024];
        snprintf(buffer, 1024, fmt, std::forward<_Args_>(args)...);
        return buffer;
    }

    class DLLDECL VulkanError : public std::runtime_error
    {
    public:
        VulkanError(const String& msg)
            : std::runtime_error(msg)
        {}

        VulkanError(const char* msg)
            : std::runtime_error(msg)
        {}

        template<typename... _Args_>
        VulkanError(const char* fmt, _Args_... args)
            : std::runtime_error(StringFormat(fmt, std::forward<_Args_>(args)...).c_str())
        {}
    };
}