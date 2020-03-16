#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    /*
     * This object will accelerate pipeline creation after the first successful loading.
     * Note: pipeline caches will have the same lifecycle with the hosting application, 
     * dont't clean pipeline caches when destroying GIDeviceVk, it is the application's decision to
     * whether store these pipeline caches into file system.
    */
    class GIPipelineCachesVk
    {
    public:

    };
}