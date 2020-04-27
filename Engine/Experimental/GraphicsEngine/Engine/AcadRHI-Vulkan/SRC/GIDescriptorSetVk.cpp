#include "GIDescriptorSetVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "GIDescriptorPoolVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDescriptorSetVk> GIDescriptorSetVk::Create(
        SharedPtr<GIDescriptorPoolVk> pool,
        SharedPtr<GIDescriptorSetLayoutVk> setLayout,
        SharedPtr<GIDescriptorSetVk> parent
    )
    {
        std::vector<VkDescriptorSetLayout> setLayouts;
        setLayouts.push_back(*setLayout);

        auto device = pool->GetDevice();
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = *pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = setLayouts.data();
        VK_CHECK(vkAllocateDescriptorSets(*device, &allocInfo, &descriptorSet));

        SharedPtr<GIDescriptorSetVk> result(new GIDescriptorSetVk(device));
        result->mDescriptorSetHandle = descriptorSet;
        result->mDescriptorSetLayout = setLayout;

        return result;
    }

    GIDescriptorSetVk::GIDescriptorSetVk(SharedPtr<GIDeviceVk> device)
        : GIDeviceObjectVk(device)
    {}

    GIDescriptorSetVk::~GIDescriptorSetVk()
    {
        auto descriptorSet = mDescriptorPool.lock();
        if (IsValid() && descriptorSet)
        {
            vkFreeDescriptorSets(*mDevice, *descriptorSet, 1, &mDescriptorSetHandle);
            mDescriptorSetHandle = VK_NULL_HANDLE;
        }
    }

    bool GIDescriptorSetVk::IsValid() const
    {
        return mDescriptorSetHandle != VK_NULL_HANDLE;
    }

    void GIDescriptorSetVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mDescriptorSetHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
            name
        );
    }

    void GIDescriptorSetVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mDescriptorSetHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
            tag
        );
    }

    GIDescriptorSetVk::operator const VkDescriptorSet& () const
    {
        return mDescriptorSetHandle;
    }

    bool GIDescriptorSetVk::IsPushDescriptorSet() const
    {
        return mDescriptorSetLayout->IsPushDescriptorSet();
    }

    SharedPtr<GIDescriptorSetLayoutVk> GIDescriptorSetVk::GetDescriptorSetLayout() const
    {
        return mDescriptorSetLayout;
    }

    void GIDescriptorSetVk::BindResource(const char* name, WeakPtr<GIResourceVk> resource, SharedPtr<GICommandBufferVk> cmdbuf)
    {
        // TODO
    }

    void GIDescriptorSetVk::BindResourceArray(const char* name, const std::vector<WeakPtr<GIResourceVk>>& resourceArray, SharedPtr<GICommandBufferVk> cmdbuf)
    {
        // TODO
    }

    void GIDescriptorSetVk::Update(SharedPtr<GICommandBufferVk> cmdbuf)
    {
        // TODO
    }
}