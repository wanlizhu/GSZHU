#pragma once

#include "GIDeviceObjectVk.h"
#include "GIIDescriptorVk.h"
#include "GIIDeviceMemoryVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferVk 
        : public GIDeviceObjectVk
        , public GIIDescriptorVk
        , public GIIDeviceMemoryVk
    {
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
    };
}