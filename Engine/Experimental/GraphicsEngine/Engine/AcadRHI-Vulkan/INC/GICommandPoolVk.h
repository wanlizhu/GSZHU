#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GICommandQueueVk;
    class GICommandBufferVk;

    class GICommandPoolVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GICommandPoolVk)
    public:
        static SharedPtr<GICommandPoolVk> Create(SharedPtr<GICommandQueueVk> queue);
        
        virtual ~GICommandPoolVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkCommandPool& () const;
        SharedPtr<GICommandBufferVk> Allocate(bool secondary = false);
        std::thread::id GetThreadId() const;
        VkQueue GetQueue() const;

    protected:
        GICommandPoolVk(SharedPtr<GICommandQueueVk> queue);
        
    private:
        VkCommandPool mCommandPoolHandle = VK_NULL_HANDLE;
        VkQueue mQueueHandle = VK_NULL_HANDLE;
        std::thread::id mThreadId;
    };
}