#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISamplerVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GISamplerVk)
    public:
        
    private:
        GISamplerVk(const VkSamplerCreateInfo& createInfo);

    private:
        VkSampler mSamplerHandle = VK_NULL_HANDLE;
        VkSamplerCreateInfo mSamplerInfo;
    };


}