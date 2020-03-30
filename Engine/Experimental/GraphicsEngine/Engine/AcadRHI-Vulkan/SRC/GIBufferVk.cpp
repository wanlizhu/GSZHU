#include "GIBufferVk.h"
#include "GIBufferViewVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIBufferVk::GIBufferVk(
        SharedPtr<GIDeviceVk> device,
        const VkBufferCreateInfo& createInfo,
        const VkMemoryAllocateInfo& allocInfo,
        const void* data,
        bool useStagingBuffer
    )
        : GIDeviceResourceVk(device)
    {
        // Erase duplicates from 'queues'
        std::vector<uint32_t> uniqueQueues = queues;
        std::sort(uniqueQueues.begin(), uniqueQueues.end());
        uniqueQueues.erase(std::unique(uniqueQueues.begin(), uniqueQueues.end()), uniqueQueues.end());

        VkBufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.size = size;
        createInfo.usage = usages;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (uniqueQueues.size() > 1)
        {
            createInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = uniqueQueues.size();
            createInfo.pQueueFamilyIndices = uniqueQueues.data();
        }
        VK_CHECK(vkCreateBuffer(*mDevice, &createInfo, nullptr, &mBuffer));

        VkMemoryRequirements memoryRequirements = {};
        vkGetBufferMemoryRequirements(*mDevice, mBuffer, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->GetMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties);
        VK_CHECK(vkAllocateMemory(*mDevice, &allocInfo, nullptr, &mDeviceMemory));

        // If the initial data has been provided, map the buffer and copy over the data.
        if (data != nullptr)
        {
            void* mappedData = nullptr;
            VK_CHECK(vkMapMemory(*mDevice, mDeviceMemory, 0, mDeviceMemorySize, 0, &mappedData));
            std::memcpy(mappedData, data, mDeviceMemorySize);

            // If host coherency hasn't been requested, do a manual flush to make writes visible.
            if (!(properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
            {
                VkMappedMemoryRange mappedRange = {};
                mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
                mappedRange.pNext = nullptr;
                mappedRange.memory = mDeviceMemory;
                mappedRange.offset = 0;
                mappedRange.size = mDeviceMemorySize;
                VK_CHECK(vkFlushMappedMemoryRanges(*mDevice, 1, &mappedRange));
            }

            vkUnmapMemory(*mDevice, mDeviceMemory);
        }

        // Attach the memory to the buffer object.
        VK_CHECK(vkBindBufferMemory(*mDevice, mBuffer, mDeviceMemory, 0));
    }

    GIBufferVk::~GIBufferVk()
    {
        if (IsValid())
        {
            vkDestroyBuffer(*mDevice, mBuffer, nullptr);
            vkFreeMemory(*mDevice, mDeviceMemory, nullptr);

            mBuffer = VK_NULL_HANDLE;
            mDeviceMemory = VK_NULL_HANDLE;
        }
    }

    bool GIBufferVk::IsValid() const
    {
        return mBuffer != VK_NULL_HANDLE && mDeviceMemory != VK_NULL_HANDLE;
    }

    void GIBufferVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mBuffer,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
            name);
    }

    void GIBufferVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mBuffer,
            VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
            tag);
    }

    GIBufferVk::operator const VkBuffer& () const
    {
        return mBuffer;
    }

    void GIBufferVk::WriteData(size_t offset, size_t size, const void* data)
    {
        // TODO: async update
    }

    void* GIBufferVk::Map(size_t offset, size_t size)
    {
        assert(mIsMapped == false);

        void* mappedData = nullptr;
        VK_CHECK(vkMapMemory(*mDevice, mDeviceMemory, offset, size, 0, &mappedData));
        mIsMapped = mappedData != nullptr;
        return mappedData;
    }

    void GIBufferVk::Unmap()
    {
        if (mIsMapped)
        {
            vkUnmapMemory(*mDevice, mDeviceMemory);
            mIsMapped = false;
        }
    }

    VkBufferUsageFlags GIBufferVk::GetUsages() const
    {
        return mUsages;
    }

    SharedPtr<GIBufferViewVk> GIBufferVk::GetBufferView(size_t offset, size_t size, VkFormat format)
    {
        auto key = ViewCacheKey(offset, size, format);
        auto view = mBufferViews.find(key);
        if (view == mBufferViews.end())
        {
            //auto newView = GIBufferViewVk::Create(shared_from_this(), offset, size, format);
            //mBufferViews[key] = newView;
            //return newView;
        }
        else
        {
            return view->second.lock();
        }
    }
}