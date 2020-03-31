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
        VkMemoryPropertyFlags properties
    )
        : GIResourceVk(device)
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
        const GITextureInfoVk& info
    )
        : GIResourceVk(device)
        , mTextureInfo(info)
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

    SharedPtr<GITextureVk> GITextureBuilderVk::Build()
    {
        auto result = SharedPtr<GITextureVk>(new GITextureVk(
            mDevice,
            mCreateInfo,
            mInitialData,
            mProperties
            ));
        assert(result->IsValid());
        return result;
    }
}