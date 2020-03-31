#include "GIDescriptorPoolVk.h"
#include "GIDescriptorSetVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "GIShaderReflectionVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDescriptorPoolVk::GIDescriptorPoolVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<GIShaderReflectionVk> reflection,
        uint32_t maxSets,
        const std::vector<VkDescriptorPoolSize>& poolSizes
    )
        : GIDeviceObjectVk(device)
        , mReflection(reflection)
        , mThreadId(std::this_thread::get_id())
    {
        VkDescriptorPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        createInfo.maxSets = maxSets;
        createInfo.poolSizeCount = (uint32_t)poolSizes.size();
        createInfo.pPoolSizes = poolSizes.data();
        VK_CHECK(vkCreateDescriptorPool(*mDevice, &createInfo, nullptr, &mDescriptorPoolHandle));
    }

    GIDescriptorPoolVk::~GIDescriptorPoolVk()
    {
        if (IsValid())
        {
            vkDestroyDescriptorPool(*mDevice, mDescriptorPoolHandle, nullptr);
            mDescriptorPoolHandle = VK_NULL_HANDLE;
        }
    }

    bool GIDescriptorPoolVk::IsValid() const
    {
        return mDescriptorPoolHandle != VK_NULL_HANDLE;
    }

    void GIDescriptorPoolVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mDescriptorPoolHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
            name
        );
    }

    void GIDescriptorPoolVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mDescriptorPoolHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
            tag
        );
    }

    GIDescriptorPoolVk::operator const VkDescriptorPool& () const
    {
        return mDescriptorPoolHandle;
    }

    std::thread::id GIDescriptorPoolVk::GetThreadId() const
    {
        return mThreadId;
    }

    SharedPtr<GIDescriptorSetVk> GIDescriptorPoolVk::Allocate(
        SharedPtr<GIDescriptorSetLayoutVk> setLayout,
        std::optional<WeakPtr<GIDescriptorSetVk>> parent
    )
    {
        return SharedPtr<GIDescriptorSetVk>(new GIDescriptorSetVk(weak_from_this(), setLayout, parent));
    }
}