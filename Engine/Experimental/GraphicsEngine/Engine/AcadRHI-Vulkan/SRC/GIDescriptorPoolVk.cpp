#include "GIDescriptorPoolVk.h"
#include "GIDescriptorSetVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "SPIRVReflection.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDescriptorPoolVk::GIDescriptorPoolVk(
        SharedPtr<GIDeviceVk> device,
        const VkDescriptorPoolCreateInfo& createInfo
        )
        : GIDeviceObjectVk(device)
        , mThreadId(std::this_thread::get_id())
    {
        VK_CHECK(vkCreateDescriptorPool(*mDevice, &createInfo, nullptr, &mDescriptorPool));
    }

    GIDescriptorPoolVk::~GIDescriptorPoolVk()
    {
        if (IsValid())
        {
            vkDestroyDescriptorPool(*mDevice, mDescriptorPool, nullptr);
            mDescriptorPool = VK_NULL_HANDLE;
        }
    }

    bool GIDescriptorPoolVk::IsValid() const
    {
        return mDescriptorPool != VK_NULL_HANDLE;
    }

    void GIDescriptorPoolVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mDescriptorPool,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
            name);
    }

    void GIDescriptorPoolVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mDescriptorPool,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
            tag);
    }

    GIDescriptorPoolVk::operator const VkDescriptorPool& () const
    {
        return mDescriptorPool;
    }

    std::thread::id GIDescriptorPoolVk::GetThreadId() const
    {
        return mThreadId;
    }

    SharedPtr<GIDescriptorSetVk> GIDescriptorPoolVk::Allocate(
        SharedPtr<GIDescriptorSetLayoutVk> setLayout,
        std::optional<WeakPtr<GIDescriptorSetVk>> parent)
    {
        return SharedPtr<GIDescriptorSetVk>(new GIDescriptorSetVk(weak_from_this(), setLayout, parent));
    }

    GIDescriptorPoolBuilderVk::GIDescriptorPoolBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
        mCreateInfo.maxSets = 0;
    }

    GIDescriptorPoolBuilderVk& GIDescriptorPoolBuilderVk::Expand(SharedPtr<SPIRVReflection> reflection)
    {
        for (const auto& poolSize : reflection->GetDescriptorPoolSizes())
        {
            Expand(poolSize);
        }
        return *this;
    }

    GIDescriptorPoolBuilderVk& GIDescriptorPoolBuilderVk::Expand(const VkDescriptorPoolSize& typeAndCount)
    {
        mPoolSizes.push_back(typeAndCount);
        return *this;
    }

    GIDescriptorPoolBuilderVk& GIDescriptorPoolBuilderVk::SetMaxSetCount(uint32_t maxSets)
    {
        mCreateInfo.maxSets = maxSets;
        return *this;
    }

    SharedPtr<GIDescriptorPoolVk> GIDescriptorPoolBuilderVk::Build()
    {
        assert(mCreateInfo.maxSets > 0);
        mCreateInfo.poolSizeCount = (uint32_t)mPoolSizes.size();
        mCreateInfo.pPoolSizes = mPoolSizes.data();

        return SharedPtr<GIDescriptorPoolVk>(new GIDescriptorPoolVk(mDevice, mCreateInfo));
    }
}