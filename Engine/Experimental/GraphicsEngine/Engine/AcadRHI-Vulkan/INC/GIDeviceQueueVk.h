#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"
#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GICommandPoolVk;

    class GIDeviceQueueVk : public GIWeakDeviceObjectVk
    {
        friend class GIDeviceBuilderVk;
        DECL_WEAK_DEVICE_OBJECT(GIDeviceQueueVk)
    public:
        static SharedPtr<GIDeviceQueueVk> Create(WeakPtr<GIDeviceVk> device, VkQueue queue, uint32_t familyIndex);
        virtual ~GIDeviceQueueVk();

        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkQueue& () const;
        void WaitIdle() const;
        uint32_t GetFamilyIndex() const;
        SharedPtr<GICommandPoolVk> GetCommandPool(); /* thread_local */

    protected:
        GIDeviceQueueVk(WeakPtr<GIDeviceVk> device, VkQueue queue, uint32_t familyIndex);
       
    private:
        VkQueue mQueueHandle = VK_NULL_HANDLE; /* VkQueue is created by driver when creating VkDevice */ 
        uint32_t mFamilyIndex = INT32_MAX;
        std::mutex mMutex; /* Multi-threads could access this device queue simultaneously */
        std::unordered_map<std::thread::id, SharedPtr<GICommandPoolVk>> mThreadCommandPools; /* VkCommandPool shall be thread_local*/
    };
}