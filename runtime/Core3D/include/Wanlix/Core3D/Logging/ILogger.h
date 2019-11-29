#pragma once

#include "Wanlix/Core3D/Types.h"

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
        virtual void Log(LogType type, StringCRef msg) const = 0;
    };
}