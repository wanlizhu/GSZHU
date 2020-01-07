#pragma once

#include "Wanlix/Common.h"

namespace Wanlix
{
    struct MonitorDesc 
    {
        String identifier;
        Uint2 resolution;
        Uint2 size;
        float ppi = 1.0f;
        bool isPrimary = true;
    };

    class Monitor
    {
    public:
        static void GetMonitorDescs(Array<MonitorDesc>& descs);
        static void PrintMonitorDesc();
    };
}