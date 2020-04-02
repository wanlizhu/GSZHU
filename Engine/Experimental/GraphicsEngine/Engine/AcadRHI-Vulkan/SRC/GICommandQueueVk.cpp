#pragma once

#include "GICommandQueueVk.h"
#include "GIDeviceVk.h"
#include "GICommandPoolVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GICommandQueueVk> GICommandQueueVk::Create(
        WeakPtr<GIDeviceVk> device,
        VkQueue queue,
        uint32_t familyIndex)
    {
        auto deviceQueue = SharedPtr<GICommandQueueVk>(new GICommandQueueVk(
            device,
            queue,
            familyIndex));
        assert(deviceQueue->IsValid());
        return deviceQueue;
    }

    GICommandQueueVk::GICommandQueueVk(
        WeakPtr<GIDeviceVk> device, 
        VkQueue queue,
        uint32_t familyIndex
    )
        : GIWeakDeviceObjectVk(device)
        , mQueueHandle(queue)
        , mFamilyIndex(familyIndex)
    {}

    GICommandQueueVk::~GICommandQueueVk()
    {}

    bool GICommandQueueVk::IsValid() const
    {
        return mQueueHandle != VK_NULL_HANDLE;
    }

    void GICommandQueueVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mQueueHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
            name
        );
    }

    void GICommandQueueVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mQueueHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
            tag
        );
    }

    GICommandQueueVk::operator const VkQueue& () const
    {
        return mQueueHandle;
    }

    void GICommandQueueVk::WaitIdle() const
    {
        vkQueueWaitIdle(mQueueHandle);
    }

    uint32_t GICommandQueueVk::GetFamilyIndex() const
    {
        return mFamilyIndex;
    }

    SharedPtr<GICommandPoolVk> GICommandQueueVk::GetCommandPool()
    {
        const auto& threadId = std::this_thread::get_id();
        const auto& it = mThreadCommandPools.find(threadId);
        if (it == mThreadCommandPools.end())
        {
            mThreadCommandPools[threadId] = GICommandPoolVk::Create(shared_from_this());
            return mThreadCommandPools[threadId];
        }
        else
        {
            return it->second;
        }
    }
}