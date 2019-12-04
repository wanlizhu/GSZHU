#pragma once

namespace Wanlix
{
    class NonCopyable
    {
    public:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        virtual ~NonCopyable() = default;

    protected:
        NonCopyable() = default;
    };
}