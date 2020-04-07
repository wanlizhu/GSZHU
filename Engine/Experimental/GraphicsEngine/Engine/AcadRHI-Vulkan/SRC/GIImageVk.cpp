#include "GIImageVk.h"
#include "GIImageViewVk.h"
#include "GISamplerVk.h"
#include "GIDeviceVk.h"
#include "GIDeviceMemoryAllocatorVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIImageVk::GIImageVk(
        SharedPtr<GIDeviceVk> device,
        const VkImageCreateInfo& createInfo,
        const void* data,
        VkMemoryPropertyFlags properties,
        EResourceState initialState
    )
        : GIResourceVk(device)
        , mResourceState(initialState)
    {
        mImageInfo.imageType = createInfo.imageType;
        mImageInfo.format = createInfo.format;
        mImageInfo.extent = createInfo.extent;
        mImageInfo.mipLevels = createInfo.mipLevels;
        mImageInfo.arrayLayers = createInfo.arrayLayers;
        mImageInfo.samples = createInfo.samples;
        mImageInfo.tiling = createInfo.tiling;
        mImageInfo.usage = createInfo.usage;
        mImageInfo.sharingMode = createInfo.sharingMode;
        mImageInfo.imageLayout = createInfo.initialLayout;

        VK_CHECK(vkCreateImage(*mDevice, &createInfo, nullptr, &mImageHandle));

        VkMemoryRequirements memoryRequirements = {};
        VkMemoryAllocateInfo allocInfo = {};

        vkGetImageMemoryRequirements(*mDevice, mImageHandle, &memoryRequirements);
        mImageInfo.sizeInBytes = memoryRequirements.size;

        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->GetMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties);
        VK_CHECK(vkAllocateMemory(*mDevice, &allocInfo, nullptr, &mMemoryHandle));

        VK_CHECK(vkBindImageMemory(*mDevice, mImageHandle, mMemoryHandle, 0));
    }

    GIImageVk::~GIImageVk()
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

    bool GIImageVk::IsValid() const
    {
        return mImageHandle != VK_NULL_HANDLE && mMemoryHandle != VK_NULL_HANDLE;
    }

    void GIImageVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mImageHandle, 
            VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
            name
        );
    }

    void GIImageVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mImageHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
            tag
        );
    }

    EResourceType GIImageVk::GetResourceType() const
    {
        return EResourceType::Image;
    }

    GIResourceStateVk& GIImageVk::GetResourceState()
    {
        return mResourceState;
    }

    void GIImageVk::TransitionState(const GIResourceStateVk& newState)
    {
        // TODO
    }

    void GIImageVk::TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateVk& newState)
    {}

    GIImageVk::operator const VkImage& () const
    {
        return mImageHandle;
    }

    GIImageInfoVk const& GIImageVk::GetInfo() const
    {
        return mImageInfo;
    }

    SharedPtr<GIImageViewVk> GIImageVk::GetTextureView(
        VkImageViewType viewType,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
    {
        auto key = GIImageViewVk::ComputeCacheIndex(viewType, format, subresource);
        auto view = mTextureViews.find(key);
        if (view == mTextureViews.end())
        {
            auto newView = GIImageViewVk::Create(shared_from_this(), viewType, format, subresource);
            mTextureViews[key] = newView;
            return newView;
        }
        else
        {
            return view->second.lock();
        }
    }

    void GIImageVk::SetOnDestroyCallback(const std::function<void()>& func)
    {
        mOnDestroyCallback = func;
    }

    void GIImageVk::CreateMipmaps(uint32_t mipLevels)
    {}

    GIImageBuilderVk::GIImageBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {}

    GIImageBuilderVk& GIImageBuilderVk::SetAllocator(SharedPtr<GIDeviceMemoryAllocatorVk> allocator)
    {
        mAllocator = allocator;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetImageType(VkImageType type)
    {
        mCreateInfo.imageType = type;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetFormat(VkFormat format)
    {
        mCreateInfo.format = format;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetExtent(VkExtent3D extent)
    {
        mCreateInfo.extent = extent;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetMipLevels(uint32_t mipLevels)
    {
        mCreateInfo.mipLevels = mipLevels;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetArrayLayers(uint32_t arrayLevels)
    {
        mCreateInfo.arrayLayers = arrayLevels;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetSamples(VkSampleCountFlagBits samples)
    {
        mCreateInfo.samples = samples;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetImageTiling(VkImageTiling tiling)
    {
        mCreateInfo.tiling = tiling;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::AddImageUsages(VkImageUsageFlags usages)
    {
        mCreateInfo.usage |= usages;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::AddMemoryProperties(VkMemoryPropertyFlags properties)
    {
        mProperties |= properties;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::AddSharedQueue(uint32_t queue)
    {
        auto it = std::find(mSharedQueues.begin(), mSharedQueues.end(), queue);
        if (it == mSharedQueues.end())
            mSharedQueues.push_back(queue);

        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetInitialLayout(VkImageLayout layout)
    {
        mCreateInfo.initialLayout = layout;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetInitialData(const void* data)
    {
        mInitialData = data;
        return *this;
    }

    GIImageBuilderVk& GIImageBuilderVk::SetInitialState(EResourceState state)
    {
        mInitialState = state;
        return *this;
    }

    SharedPtr<GIImageVk> GIImageBuilderVk::Build()
    {
        auto result = SharedPtr<GIImageVk>(new GIImageVk(
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