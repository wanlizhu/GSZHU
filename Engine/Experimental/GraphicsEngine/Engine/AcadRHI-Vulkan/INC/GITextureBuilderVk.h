#pragma once

#include "GICommonVk.h"
#include "GIResourceStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GITextureVk;

    class GITextureBuilderVk
    {
    public:
        GITextureBuilderVk(SharedPtr<GIDeviceVk> device);
        GITextureBuilderVk& SetImageType(VkImageType type);
        GITextureBuilderVk& SetFormat(VkFormat format);
        GITextureBuilderVk& SetExtent(VkExtent3D extent);
        GITextureBuilderVk& SetMipLevels(uint32_t mipLevels);
        GITextureBuilderVk& SetArrayLayers(uint32_t arrayLevels);
        GITextureBuilderVk& SetSamples(VkSampleCountFlagBits samples);
        GITextureBuilderVk& SetImageTiling(VkImageTiling tiling);
        GITextureBuilderVk& AddImageUsages(VkImageUsageFlags usages);
        GITextureBuilderVk& AddMemoryProperties(VkMemoryPropertyFlags properties);
        GITextureBuilderVk& AddSharedQueue(uint32_t queue);
        GITextureBuilderVk& SetInitialLayout(VkImageLayout layout);
        GITextureBuilderVk& SetInitialData(const void* data);
        GITextureBuilderVk& SetInitialState(EResourceState state);
        SharedPtr<GITextureVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkImageCreateInfo mCreateInfo = {};
        VkMemoryPropertyFlags mProperties = 0;

        const void* mInitialData = nullptr;
        EResourceState mInitialState = EResourceState::Undefined;
        std::vector<uint32_t> mSharedQueues;
    };

    class GITextureBuilderVMA
    {
    public:
        // TODO
    };
}