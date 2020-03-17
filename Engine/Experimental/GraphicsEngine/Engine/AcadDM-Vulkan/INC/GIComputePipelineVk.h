#pragma once

#include "GIIPipelineVk.h"
#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIComputePipelineVk
        : public GIDeviceObjectVk
        , public GIIPipelineVk
    {
        friend class GIComputePipelineBuilderVk;
        DECL_DEVICE_OBJECT(GIComputePipelineVk)
    public:
        
    };

    class GIComputePipelineBuilderVk
    {
    public:
       
    };
}