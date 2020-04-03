#pragma once

#include "GICommonVk.h"
#include "GIResourceStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferVk;
    class GIDeviceVk;

    /*
     * This will invoke the first constructor of GIBufferVk
    */
    class GIBufferBuilderVk
    {
    public:
        GIBufferBuilderVk(SharedPtr<GIDeviceVk> device);
        GIBufferBuilderVk& SetSize(VkDeviceSize size);
        GIBufferBuilderVk& AddBufferUsages(VkBufferUsageFlags usages);
        GIBufferBuilderVk& AddMemoryProperties(VkMemoryPropertyFlags properties);
        GIBufferBuilderVk& AddSharedQueue(uint32_t queue);
        GIBufferBuilderVk& SetInitialData(const void* data);
        GIBufferBuilderVk& SetInitialState(EResourceState state);
        SharedPtr<GIBufferVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkBufferCreateInfo mCreateInfo = {};
        VkMemoryPropertyFlags mProperties = 0;

        const void* mInitialData = nullptr;
        EResourceState mInitialState = EResourceState::Undefined;
        std::vector<uint32_t> mSharedQueues;
    };

    /*
     * This will invoke the second constructor of GIBufferVk
    */
    class GIBufferBuilderVMA
    {
    public:
        // TODO
    };
}