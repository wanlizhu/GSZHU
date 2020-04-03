#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GISamplerVk;

    class GISamplerBuilderVk
    {
    public:
        GISamplerBuilderVk(SharedPtr<GIDeviceVk> device);
        GISamplerBuilderVk& SetMagFilter(VkFilter filter);
        GISamplerBuilderVk& SetMinFilter(VkFilter filter);
        GISamplerBuilderVk& SetMipmapMode(VkSamplerMipmapMode mode);
        GISamplerBuilderVk& SetAddressModeU(VkSamplerAddressMode addressU);
        GISamplerBuilderVk& SetAddressModeV(VkSamplerAddressMode addressV);
        GISamplerBuilderVk& SetAddressModeW(VkSamplerAddressMode addressW);
        GISamplerBuilderVk& SetMipLodBias(float mipLodBias);
        GISamplerBuilderVk& SetAnisotropy(bool enable, float maxAnisotropy);
        GISamplerBuilderVk& SetCompareOp(bool enable, VkCompareOp compareOp);
        GISamplerBuilderVk& SetLod(float minLod, float maxLod);
        GISamplerBuilderVk& SetBorderColor(VkBorderColor color);
        GISamplerBuilderVk& SetUnnormalizedCoordinates(bool value);
        SharedPtr<GISamplerVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkSamplerCreateInfo mCreateInfo = {};
    };
}