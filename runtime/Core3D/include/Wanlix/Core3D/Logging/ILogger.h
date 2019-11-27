#pragma once

#include <string>

namespace Wanlix
{
    enum LogType
    {
        LogInfo,
        LogWarning,
        LogError,
    };

    class ILogger
    {
    public:
        virtual ~ILogger() = default;
        virtual void Log(LogType type, const std::string& msg) const = 0;
    };
}