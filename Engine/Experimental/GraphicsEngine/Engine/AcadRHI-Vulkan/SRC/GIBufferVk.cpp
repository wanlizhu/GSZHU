#include "GIBufferVk.h"
#include "GIBufferViewVk.h"
#include "GIDeviceVk.h"
#include "GIDeviceMemoryAllocatorVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIBufferVk> GIBufferVk::Create(
        SharedPtr<GIDeviceVk> device,
        VkDeviceSize size,
        VkBufferUsageFlagBits usage,
        std::vector<uint32_t> sharingQueues,
        VkMemoryPropertyFlags properties,
        const void* initialData,
        EResourceState initialState
    )
    {
        VkBufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.size = size;
        createInfo.usage = usage;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;

        if (!sharingQueues.empty())
        {
            createInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = (uint32_t)sharingQueues.size();
            createInfo.pQueueFamilyIndices = sharingQueues.data();
        }

        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VK_CHECK(vkCreateBuffer(*device, &createInfo, nullptr, &buffer));

        VkMemoryRequirements requirements = {};
        VkMemoryAllocateInfo allocInfo = {};

        vkGetBufferMemoryRequirements(*device, buffer, &requirements);
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = requirements.size;
        allocInfo.memoryTypeIndex = device->GetMemoryTypeIndex(requirements.memoryTypeBits, properties);
        VK_CHECK(vkAllocateMemory(*device, &allocInfo, nullptr, &memory));
        VK_CHECK(vkBindBufferMemory(*device, buffer, memory, 0));

        SharedPtr<GIBufferVk> result(new GIBufferVk(device));
        result->mBufferHandle = buffer;
        result->mMemoryHandle = memory;
        result->mMemoryProperties = properties;
        result->mSizeInBytes = requirements.size;
        result->mUsage = usage;
        result->mResourceState = initialState;

        if (initialData != nullptr)
        {
            result->UpdateData(0, requirements.size, initialData, nullptr);
        }

        return result;
    }

    GIBufferVk::GIBufferVk(SharedPtr<GIDeviceVk> device)
        : GIResourceVk(device)
    {}

    GIBufferVk::~GIBufferVk()
    {
        if (mOnDestroyCallback)
        {
            mOnDestroyCallback();
        }

        if (IsValid())
        {
            vkDestroyBuffer(*mDevice, mBufferHandle, nullptr);
            vkFreeMemory(*mDevice, mMemoryHandle, nullptr);

            mBufferHandle = VK_NULL_HANDLE;
            mMemoryHandle = VK_NULL_HANDLE;
        }
    }

    bool GIBufferVk::IsValid() const
    {
        return mBufferHandle != VK_NULL_HANDLE && mMemoryHandle != VK_NULL_HANDLE;
    }

    void GIBufferVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mBufferHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
            name
        );
    }

    void GIBufferVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mBufferHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
            tag
        );
    }

    EResourceType GIBufferVk::GetResourceType() const
    {
        return EResourceType::Buffer;
    }

    EResourceState GIBufferVk::GetResourceState() const
    {
        return mResourceState;
    }

    void GIBufferVk::TransitionState(const EResourceState& newState)
    {
        //TODO
    }

    GIBufferVk::operator const VkBuffer& () const
    {
        return mBufferHandle;
    }

    VkDeviceMemory GIBufferVk::GetMemory() const
    {
        return mMemoryHandle;
    }

    VkMemoryPropertyFlags GIBufferVk::GetMemoryProperties() const
    {
        return mMemoryProperties;
    }

    VkDeviceSize GIBufferVk::GetSizeInBytes() const
    {
        return mSizeInBytes;
    }

    VkBufferUsageFlags GIBufferVk::GetUsage() const
    {
        return mUsage;
    }

    SharedPtr<GIBufferViewVk> GIBufferVk::GetBufferView(size_t offset, size_t size, VkFormat format)
    {
        auto key = GIBufferViewVk::ComputeCacheIndex(offset, size, format);
        auto view = mBufferViews.find(key);
        if (view == mBufferViews.end())
        {
            auto newView = GIBufferViewVk::Create(shared_from_this(), offset, size, format);
            mBufferViews[key] = newView;
            return newView;
        }
        else
        {
            return view->second.lock();
        }
    }

    void GIBufferVk::SetOnDestroyCallback(const std::function<void()>& func)
    {
        mOnDestroyCallback = func;
    }

    void* GIBufferVk::Map(size_t offset, size_t size)
    {
        assert(mIsMapped == false);

        void* mappedData = nullptr;
        VK_CHECK(vkMapMemory(*mDevice, mMemoryHandle, offset, size, 0, &mappedData));
        mIsMapped = mappedData != nullptr;
        return mappedData;
    }

    void GIBufferVk::Unmap()
    {
        if (mIsMapped)
        {
            vkUnmapMemory(*mDevice, mMemoryHandle);
            mIsMapped = false;
        }
    }

    void GIBufferVk::UpdateData(
        size_t offset, 
        size_t size,
        const void* data,
        SharedPtr<GICommandBufferVk> cmdbuf
    )
    {
        // TODO
    }
}