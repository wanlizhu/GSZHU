#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"
#include "GIDeviceQueueVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GICommandPoolVk> GICommandPoolVk::Create(SharedPtr<GIDeviceQueueVk> queue)
    {
        auto pool = SharedPtr<GICommandPoolVk>(new GICommandPoolVk(queue));
        assert(pool->IsValid());
        return pool;
    }

    GICommandPoolVk::GICommandPoolVk(SharedPtr<GIDeviceQueueVk> queue)
        : GIDeviceObjectVk(queue->GetDevice().lock())
        , mQueue(*queue)
        , mThreadId(std::this_thread::get_id())
    {
        VkCommandPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = queue->GetFamilyIndex();
        VK_CHECK(vkCreateCommandPool(*mDevice, &createInfo, nullptr, &mCommandPool));
    }

    GICommandPoolVk::~GICommandPoolVk()
    {
        vkDestroyCommandPool(*mDevice, mCommandPool, nullptr);
        mCommandPool = VK_NULL_HANDLE;
    }

    bool GICommandPoolVk::IsValid() const
    {
        return mCommandPool != VK_NULL_HANDLE;
    }

    void GICommandPoolVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            (void*)mCommandPool,
            VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
            name);
    }

    void GICommandPoolVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            (void*)mCommandPool,
            VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
            tag);
    }

    GICommandPoolVk::operator const VkCommandPool& () const
    {
        return mCommandPool;
    }

    SharedPtr<GICommandBufferVk> GICommandPoolVk::Allocate(bool secondary)
    {
        return SharedPtr<GICommandBufferVk>(new GICommandBufferVk(shared_from_this(), secondary));
    }

    std::thread::id GICommandPoolVk::GetThreadId() const
    {
        return mThreadId;
    }

    VkQueue GICommandPoolVk::GetQueue() const
    {
        return mQueue;
    }
}