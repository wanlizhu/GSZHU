#pragma once

#include "GIDescriptorSetVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVReflection;
    class GIDescriptorSetVk;

    /*
     * Regardless of whether sets are individually freed by calling vkFreeDescriptorSets() or freed
     * in bulk by calling vkResetDescriptorPool(), care must be taken to ensure that sets are not
     * referenced after they have been freed. In particular, any command buffer containing commands that
     * might reference descriptor sets that are to be freed should either have completed execution or should
     * be discarded without submission.
    */
    class GIDescriptorPoolVk : public GIDeviceObjectVk
    {
        friend class GIDescriptorPoolBuilderVk;
        DECL_DEVICE_OBJECT(GIDescriptorPoolVk)
    public:
        virtual ~GIDescriptorPoolVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkDescriptorPool& () const;
        std::thread::id GetThreadId() const;
        SharedPtr<GIDescriptorSetVk> Allocate(
            SharedPtr<GIDescriptorSetLayoutVk> setLayout, 
            std::optional<WeakPtr<GIDescriptorSetVk>> parent = std::nullopt);
        
    protected:
        GIDescriptorPoolVk(
            SharedPtr<GIDeviceVk> device,
            const VkDescriptorPoolCreateInfo& createInfo);

    private:
        VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
        std::thread::id mThreadId;
    };

    class GIDescriptorPoolBuilderVk
    {
    public:
        GIDescriptorPoolBuilderVk(SharedPtr<GIDeviceVk> device);
        
        GIDescriptorPoolBuilderVk& Expand(SharedPtr<SPIRVReflection> reflection);
        GIDescriptorPoolBuilderVk& Expand(const VkDescriptorPoolSize& typeAndCount);
        GIDescriptorPoolBuilderVk& SetMaxSetCount(uint32_t maxSets);

        SharedPtr<GIDescriptorPoolVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkDescriptorPoolCreateInfo mCreateInfo = {};

        std::vector<VkDescriptorPoolSize> mPoolSizes;
    };
}