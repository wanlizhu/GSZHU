#include "GIBufferBuilderVk.h"
#include "GIBufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIBufferBuilderVk::GIBufferBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
        mCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    GIBufferBuilderVk& GIBufferBuilderVk::SetSize(VkDeviceSize size)
    {
        mCreateInfo.size = size;
        return *this;
    }

    GIBufferBuilderVk& GIBufferBuilderVk::AddBufferUsages(VkBufferUsageFlags usages)
    {
        mCreateInfo.usage |= usages;
        return *this;
    }

    GIBufferBuilderVk& GIBufferBuilderVk::AddMemoryProperties(VkMemoryPropertyFlags properties)
    {
        mProperties |= properties;
        return *this;
    }

    GIBufferBuilderVk& GIBufferBuilderVk::AddSharedQueue(uint32_t queue)
    {
        auto it = std::find(mSharedQueues.begin(), mSharedQueues.end(), queue);
        if (it == mSharedQueues.end())
            mSharedQueues.push_back(queue);

        return *this;
    }

    GIBufferBuilderVk& GIBufferBuilderVk::SetInitialData(const void* data)
    {
        mInitialData = data;
        return *this;
    }

    GIBufferBuilderVk& GIBufferBuilderVk::SetInitialState(EResourceState state)
    {
        mInitialState = state;
        return *this;
    }

    SharedPtr<GIBufferVk> GIBufferBuilderVk::Build()
    {
        auto result = SharedPtr<GIBufferVk>(new GIBufferVk(
            mDevice,
            mCreateInfo,
            mInitialData,
            mProperties,
            mInitialState
            ));
        assert(result->IsValid());
        return result;
    }
}