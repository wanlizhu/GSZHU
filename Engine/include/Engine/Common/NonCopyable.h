#pragma once

#include "Engine/Common/Config.h"

namespace ZHU
{
    class ZHU_API NonCopyable
    {
    public:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = default;
        NonCopyable& operator=(NonCopyable&&) = default;

    protected:
        NonCopyable() = default;
        virtual ~NonCopyable() = default;
    };
}