#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorSetLayoutVk;
    class GIDescriptorPoolVk;
    class GIDeviceResourceVk;
    class GICommandBufferVk;

    class GIDescriptorSetVk : public GIDeviceObjectVk
    {
        friend class GIDescriptorPoolVk;
        DECL_DEVICE_OBJECT(GIDescriptorSetVk)
    public:
        virtual ~GIDescriptorSetVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkDescriptorSet& () const;
        bool IsPushDescriptorSet() const;
        SharedPtr<GIDescriptorSetLayoutVk> GetDescriptorSetLayout() const;
        
        void BindResource(const char* name, WeakPtr<GIDeviceResourceVk> resource, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        void BindResourceArray(const char* name, const std::vector<WeakPtr<GIDeviceResourceVk>>& resourceArray, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        void Update(SharedPtr<GICommandBufferVk> cmdbuf = nullptr);

    private:
        GIDescriptorSetVk(
            WeakPtr<GIDescriptorPoolVk> pool,
            SharedPtr<GIDescriptorSetLayoutVk> setLayout,
            std::optional<WeakPtr<GIDescriptorSetVk>> parent
        );

    private:
        VkDescriptorSet mDescriptorSet = VK_NULL_HANDLE; // Must be NULL for push-descriptor-set
        SharedPtr<GIDescriptorSetLayoutVk> mDescriptorSetLayout;
        WeakPtr<GIDescriptorPoolVk> mDescriptorPool;
        std::optional<WeakPtr<GIDescriptorSetVk>> mParent;
        
        std::unordered_map<std::string, WeakPtr<GIDeviceResourceVk>> mBoundResources;
        std::unordered_map<std::string, std::vector<WeakPtr<GIDeviceResourceVk>>> mBoundResourceArrays;
        std::vector<VkWriteDescriptorSet> mPendingUpdates;
    };
}