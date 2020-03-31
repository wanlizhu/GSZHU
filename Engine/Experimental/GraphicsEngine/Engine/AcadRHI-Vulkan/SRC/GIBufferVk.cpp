#include "GIBufferVk.h"
#include "GIBufferViewVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIBufferVk::GIBufferVk(
        SharedPtr<GIDeviceVk> device,
        const VkBufferCreateInfo& createInfo,
        const void* data,
        VkMemoryPropertyFlags properties
    )
        : GIResourceVk(device)
        , mBufferUsages(createInfo.usage)
    {
        VK_CHECK(vkCreateBuffer(*mDevice, &createInfo, nullptr, &mBufferHandle));

        VkMemoryRequirements memoryRequirements = {};
        VkMemoryAllocateInfo allocInfo = {};

        vkGetBufferMemoryRequirements(*mDevice, mBufferHandle, &memoryRequirements);
        mSizeInBytes = memoryRequirements.size;

        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->GetMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties);
        VK_CHECK(vkAllocateMemory(*mDevice, &allocInfo, nullptr, &mMemoryHandle));
        
        UpdateData(0, mSizeInBytes, data, nullptr);

        // Attach the memory to the buffer object.
        VK_CHECK(vkBindBufferMemory(*mDevice, mBufferHandle, mMemoryHandle, 0));
    }

    GIBufferVk::GIBufferVk(
        SharedPtr<GIDeviceVk> device,
        VkBuffer buffer,
        VkDeviceMemory memory,
        VkDeviceSize size,
        VkBufferUsageFlags usages
    )
        : GIResourceVk(device)
        , mBufferHandle(buffer)
        , mMemoryHandle(memory)
        , mSizeInBytes(size)
        , mBufferUsages(usages)
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

    GIResourceStateVk& GIBufferVk::GetResourceState()
    {
        return mResourceState;
    }

    GIBufferVk::operator const VkBuffer& () const
    {
        return mBufferHandle;
    }

    VkDeviceSize GIBufferVk::GetSize() const
    {
        return mSizeInBytes;
    }

    VkDeviceMemory GIBufferVk::GetDeviceMemory() const
    {
        return mMemoryHandle;
    }

    VkBufferUsageFlags GIBufferVk::GetUsages() const
    {
        return mBufferUsages;
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

    void GIBufferVk::UpdateData(size_t offset, size_t size, const void* data, SharedPtr<GICommandBufferVk> cmdbuf)
    {
        // TODO
    }

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

    SharedPtr<GIBufferVk> GIBufferBuilderVk::Build()
    {
        auto result = SharedPtr<GIBufferVk>(new GIBufferVk(
            mDevice,
            mCreateInfo,
            mInitialData,
            mProperties
            ));
        assert(result->IsValid());
        return result;
    }
}