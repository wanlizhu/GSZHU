#include "GITextureVk.h"
#include "GITextureViewVk.h"
#include "GISamplerVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GITextureVk::GITextureVk(
        SharedPtr<GIDeviceVk> device,
        const VkImageCreateInfo& createInfo,
        const void* data,
        VkMemoryPropertyFlags properties,
        EResourceState initialState
    )
        : GIResourceVk(device)
        , mResourceState(initialState)
    {
        mTextureInfo.imageType = createInfo.imageType;
        mTextureInfo.format = createInfo.format;
        mTextureInfo.extent = createInfo.extent;
        mTextureInfo.mipLevels = createInfo.mipLevels;
        mTextureInfo.arrayLayers = createInfo.arrayLayers;
        mTextureInfo.samples = createInfo.samples;
        mTextureInfo.tiling = createInfo.tiling;
        mTextureInfo.usage = createInfo.usage;
        mTextureInfo.sharingMode = createInfo.sharingMode;
        mTextureInfo.imageLayout = createInfo.initialLayout;

        VK_CHECK(vkCreateImage(*mDevice, &createInfo, nullptr, &mImageHandle));

        VkMemoryRequirements memoryRequirements = {};
        VkMemoryAllocateInfo allocInfo = {};

        vkGetImageMemoryRequirements(*mDevice, mImageHandle, &memoryRequirements);
        mTextureInfo.sizeInBytes = memoryRequirements.size;

        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->GetMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties);
        VK_CHECK(vkAllocateMemory(*mDevice, &allocInfo, nullptr, &mMemoryHandle));

        VK_CHECK(vkBindImageMemory(*mDevice, mImageHandle, mMemoryHandle, 0));
    }

    GITextureVk::GITextureVk(
        SharedPtr<GIDeviceVk> device,
        const GITextureInfoVk& info,
        EResourceState initialState
    )
        : GIResourceVk(device)
        , mTextureInfo(info)
        , mResourceState(initialState)
    {}

    GITextureVk::~GITextureVk()
    {
        if (mOnDestroyCallback)
        {
            mOnDestroyCallback();
        }

        if (IsValid())
        {
            vkDestroyImage(*mDevice, mImageHandle, nullptr);
            vkFreeMemory(*mDevice, mMemoryHandle, nullptr);

            mImageHandle = VK_NULL_HANDLE;
            mMemoryHandle = VK_NULL_HANDLE;
        }
    }

    bool GITextureVk::IsValid() const
    {
        return mImageHandle != VK_NULL_HANDLE && mMemoryHandle != VK_NULL_HANDLE;
    }

    void GITextureVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mImageHandle, 
            VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
            name
        );
    }

    void GITextureVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mImageHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
            tag
        );
    }

    EResourceType GITextureVk::GetResourceType() const
    {
        return EResourceType::Image;
    }

    GIResourceStateVk& GITextureVk::GetResourceState()
    {
        return mResourceState;
    }

    void GITextureVk::TransitionState(const GIResourceStateVk& newState)
    {
        // TODO
    }

    void GITextureVk::TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateVk& newState)
    {}

    GITextureVk::operator const VkImage& () const
    {
        return mImageHandle;
    }

    GITextureInfoVk const& GITextureVk::GetInfo() const
    {
        return mTextureInfo;
    }

    SharedPtr<GISamplerVk> GITextureVk::GetSampler() const
    {
        return mSampler;
    }

    SharedPtr<GITextureViewVk> GITextureVk::GetTextureView(
        VkImageViewType viewType,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
    {
        auto key = GITextureViewVk::ComputeCacheIndex(viewType, format, subresource);
        auto view = mTextureViews.find(key);
        if (view == mTextureViews.end())
        {
            auto newView = GITextureViewVk::Create(shared_from_this(), viewType, format, subresource);
            mTextureViews[key] = newView;
            return newView;
        }
        else
        {
            return view->second.lock();
        }
    }

    void GITextureVk::SetSampler(SharedPtr<GISamplerVk> sampler)
    {
        mSampler = sampler;
    }

    void GITextureVk::SetOnDestroyCallback(const std::function<void()>& func)
    {
        mOnDestroyCallback = func;
    }

    void GITextureVk::CreateMipmaps(uint32_t mipLevels)
    {}
}