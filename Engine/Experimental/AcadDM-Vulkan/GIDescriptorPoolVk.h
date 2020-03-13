#pragma once

#include "GIDescriptorSetVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorPoolVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIDescriptorPoolVk)
    public:
        virtual ~GIDescriptorPoolVk();

        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

    protected:
        GIDescriptorPoolVk(const SharedPtr<GIDeviceVk>& device, const VkDescriptorPoolCreateInfo& createInfo);
        GIDescriptorPoolVk(const GIDescriptorPoolVk&) = delete;
        GIDescriptorPoolVk(GIDescriptorPoolVk&&) = default;
        GIDescriptorPoolVk& operator=(const GIDescriptorPoolVk&) = delete;
        GIDescriptorPoolVk& operator=(GIDescriptorPoolVk&&) = default;

    private:
        VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    };
}