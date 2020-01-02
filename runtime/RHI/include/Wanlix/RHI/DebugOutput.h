#pragma once

#include "Common.h"

namespace Wanlix
{
    enum class DebugSeverity
    {
        Info = 0,
        Warning,
        Error,
        FatalError,
    };

    struct DebugLocation
    {
        String fileName;
        String funcName;
        Uint   line;
    };

    using DebugMessageCallback = void(*)(DebugSeverity, String const&, DebugLocation const&);

    void SetDebugMessageCallback(DebugMessageCallback callback);
}