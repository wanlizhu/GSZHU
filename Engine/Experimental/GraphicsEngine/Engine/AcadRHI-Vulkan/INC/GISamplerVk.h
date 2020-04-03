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
}