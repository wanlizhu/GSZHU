#include "GITextureBuilderVk.h"
#include "GITextureVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GITextureBuilderVk::GITextureBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {}

    GITextureBuilderVk& GITextureBuilderVk::SetImageType(VkImageType type)
    {
        mCreateInfo.imageType = type;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetFormat(VkFormat format)
    {
        mCreateInfo.format = format;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetExtent(VkExtent3D extent)
    {
        mCreateInfo.extent = extent;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetMipLevels(uint32_t mipLevels)
    {
        mCreateInfo.mipLevels = mipLevels;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetArrayLayers(uint32_t arrayLevels)
    {
        mCreateInfo.arrayLayers = arrayLevels;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetSamples(VkSampleCountFlagBits samples)
    {
        mCreateInfo.samples = samples;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetImageTiling(VkImageTiling tiling)
    {
        mCreateInfo.tiling = tiling;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::AddImageUsages(VkImageUsageFlags usages)
    {
        mCreateInfo.usage |= usages;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::AddMemoryProperties(VkMemoryPropertyFlags properties)
    {
        mProperties |= properties;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::AddSharedQueue(uint32_t queue)
    {
        auto it = std::find(mSharedQueues.begin(), mSharedQueues.end(), queue);
        if (it == mSharedQueues.end())
            mSharedQueues.push_back(queue);

        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetInitialLayout(VkImageLayout layout)
    {
        mCreateInfo.initialLayout = layout;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetInitialData(const void* data)
    {
        mInitialData = data;
        return *this;
    }

    GITextureBuilderVk& GITextureBuilderVk::SetInitialState(EResourceState state)
    {
        mInitialState = state;
        return *this;
    }

    SharedPtr<GITextureVk> GITextureBuilderVk::Build()
    {
        auto result = SharedPtr<GITextureVk>(new GITextureVk(
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