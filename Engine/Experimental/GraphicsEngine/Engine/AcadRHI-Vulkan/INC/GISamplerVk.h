#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISamplerVk : public GIDeviceObjectVk
    {
        friend class GISamplerBuilderVk;
        DECL_DEVICE_OBJECT(GISamplerVk)
    public:
        virtual ~GISamplerVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkSampler& () const;
        VkSamplerCreateInfo const& GetSamplerInfo() const;

    private:
        GISamplerVk(
            SharedPtr<GIDeviceVk> device,
            const VkSamplerCreateInfo& createInfo
        );

    private:
        VkSampler mSamplerHandle = VK_NULL_HANDLE;
        VkSamplerCreateInfo mSamplerInfo;
    };

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