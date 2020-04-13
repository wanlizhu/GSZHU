#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"
#include "GICommandQueueVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GICommandPoolVk> GICommandPoolVk::Create(SharedPtr<GICommandQueueVk> queue)
    {
        auto device = queue->GetDevice().lock();
        VkCommandPool pool = VK_NULL_HANDLE;

        VkCommandPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = queue->GetFamilyIndex();
        VK_CHECK(vkCreateCommandPool(*device, &createInfo, nullptr, &pool));

        SharedPtr<GICommandPoolVk> result(new GICommandPoolVk(device));
        result->mCommandPoolHandle = pool;
        result->mQueueHandle = *queue;
        result->mThreadId = std::this_thread::get_id();

        return result;
    }

    GICommandPoolVk::GICommandPoolVk(SharedPtr<GIDeviceVk> device)
        : GIDeviceObjectVk(device)
    {}

    GICommandPoolVk::~GICommandPoolVk()
    {
        vkDestroyCommandPool(*mDevice, mCommandPoolHandle, nullptr);
        mCommandPoolHandle = VK_NULL_HANDLE;
    }

    bool GICommandPoolVk::IsValid() const
    {
        return mCommandPoolHandle != VK_NULL_HANDLE;
    }

    void GICommandPoolVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            (void*)mCommandPoolHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
            name
        );
    }

    void GICommandPoolVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            (void*)mCommandPoolHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
            tag
        );
    }

    GICommandPoolVk::operator const VkCommandPool& () const
    {
        return mCommandPoolHandle;
    }

    SharedPtr<GICommandBufferVk> GICommandPoolVk::Allocate(bool secondary, uint32_t threshold)
    {
        return GICommandBufferVk::Create(shared_from_this(), secondary, threshold);
    }

    std::thread::id GICommandPoolVk::GetThreadId() const
    {
        return mThreadId;
    }

    VkQueue GICommandPoolVk::GetQueue() const
    {
        return mQueueHandle;
    }
}