#pragma once

#include "GIDescriptorSetVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIShaderReflectionVk;
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
        static SharedPtr<GIDescriptorPoolVk> Create(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<GIShaderReflectionVk> reflection,
            uint32_t maxSets,
            const std::vector<VkDescriptorPoolSize>& poolSizes
        );

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
            SharedPtr<GIShaderReflectionVk> reflection,
            uint32_t maxSets,
            const std::vector<VkDescriptorPoolSize>& poolSizes
        );

    private:
        SharedPtr<GIShaderReflectionVk> mReflection;
        VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
        std::thread::id mThreadId;
    };
}