#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorSetLayoutVk;
    class GIDescriptorResourceVk;
    class GIDescriptorPoolVk;

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
        
        void BindResource(uint32_t binding, WeakPtr<GIDescriptorResourceVk> resource);
        void SetParentDescriptorSet(WeakPtr<GIDescriptorSetVk> set);
        void Update();

    private:
        GIDescriptorSetVk(
            WeakPtr<GIDescriptorPoolVk> pool,
            SharedPtr<GIDescriptorSetLayoutVk> setLayout,
            std::optional<WeakPtr<GIDescriptorSetVk>> parent);

    private:
        VkDescriptorSet mDescriptorSet = VK_NULL_HANDLE; // Must be VK_NULL_HANDLE for push-descriptor-set
        SharedPtr<GIDescriptorSetLayoutVk> mDescriptorSetLayout;
        WeakPtr<GIDescriptorPoolVk> mDescriptorPool;
        std::optional<WeakPtr<GIDescriptorSetVk>> mParent;
        
        std::unordered_map<uint32_t, WeakPtr<GIDescriptorResourceVk>> mBoundResources;
        std::vector<WeakPtr<GIDescriptorResourceVk>> mPendingUpdates;
    };
}