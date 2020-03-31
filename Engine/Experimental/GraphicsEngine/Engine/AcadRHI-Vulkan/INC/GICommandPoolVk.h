#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceQueueVk;
    class GICommandBufferVk;

    class GICommandPoolVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GICommandPoolVk)
    public:
        static SharedPtr<GICommandPoolVk> Create(SharedPtr<GIDeviceQueueVk> queue);
        virtual ~GICommandPoolVk();

        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkCommandPool& () const;
        SharedPtr<GICommandBufferVk> Allocate(bool secondary = false);
        std::thread::id GetThreadId() const;
        VkQueue GetQueue() const;

    protected:
        GICommandPoolVk(SharedPtr<GIDeviceQueueVk> queue);
        
    private:
        VkCommandPool mCommandPoolHandle = VK_NULL_HANDLE;
        VkQueue mQueueHandle = VK_NULL_HANDLE;
        std::thread::id mThreadId;
    };
}