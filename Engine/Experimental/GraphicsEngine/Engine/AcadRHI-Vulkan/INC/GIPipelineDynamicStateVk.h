#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIPipelineDynamicStateVk
    {
    public:
        virtual ~GIPipelineDynamicStateVk() = default;
        virtual VkDynamicState GetType() const = 0;
        virtual bool IsPendingUpdate() const = 0;
    };
}