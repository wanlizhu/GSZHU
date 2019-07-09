#pragma once
#include <ZHUENGINE/Config.h>

namespace ZHU
{
    // Without ZHU_API,
    // warning C4275: non dll-interface class 'ZHU::NonCopyable' 
    // used as base for dll-interface class '...'
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