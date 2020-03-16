#pragma once

#include "GIDeviceQueueVk.h"
#include "GIDeviceVk.h"
#include "GICommandPoolVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDeviceQueueVk> GIDeviceQueueVk::Create(
        WeakPtr<GIDeviceVk> device,
        VkQueue queue,
        uint32_t familyIndex)
    {
        auto deviceQueue = SharedPtr<GIDeviceQueueVk>(new GIDeviceQueueVk(
            device,
            queue,
            familyIndex));
        assert(deviceQueue->IsValid());
        return deviceQueue;
    }

    GIDeviceQueueVk::GIDeviceQueueVk(
        WeakPtr<GIDeviceVk> device, 
        VkQueue queue,
        uint32_t familyIndex
    )
        : GIWeakDeviceObjectVk(device)
        , mQueue(queue)
        , mFamilyIndex(familyIndex)
    {}

    GIDeviceQueueVk::~GIDeviceQueueVk()
    {}

    bool GIDeviceQueueVk::IsValid() const
    {
        return mQueue != VK_NULL_HANDLE;
    }

    void GIDeviceQueueVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mQueue,
            VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
            name);
    }

    void GIDeviceQueueVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mQueue,
            VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
            tag);
    }

    GIDeviceQueueVk::operator const VkQueue& () const
    {
        return mQueue;
    }

    void GIDeviceQueueVk::WaitIdle() const
    {
        vkQueueWaitIdle(mQueue);
    }

    uint32_t GIDeviceQueueVk::GetFamilyIndex() const
    {
        return mFamilyIndex;
    }

    SharedPtr<GICommandPoolVk> GIDeviceQueueVk::GetCommandPool()
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