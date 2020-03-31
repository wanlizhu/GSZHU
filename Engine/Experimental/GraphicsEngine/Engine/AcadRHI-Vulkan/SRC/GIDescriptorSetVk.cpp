#include "GIDescriptorSetVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "GIDescriptorPoolVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDescriptorSetVk::GIDescriptorSetVk(
        WeakPtr<GIDescriptorPoolVk> pool,
        SharedPtr<GIDescriptorSetLayoutVk> setLayout,
        std::optional<WeakPtr<GIDescriptorSetVk>> parent
    )
        : GIDeviceObjectVk(pool.lock()->GetDevice())
        , mDescriptorSetLayout(setLayout)
        , mDescriptorPool(pool)
        , mParent(parent)
    {
        std::vector<VkDescriptorSetLayout> setLayouts;
        setLayouts.push_back(*setLayout);

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = *mDescriptorPool.lock();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = setLayouts.data();

        VK_CHECK(vkAllocateDescriptorSets(*mDevice, &allocInfo, &mDescriptorSetHandle));
    }

    GIDescriptorSetVk::~GIDescriptorSetVk()
    {
        auto pool = mDescriptorPool.lock();
        if (IsValid() && pool)
        {
            vkFreeDescriptorSets(*mDevice, *pool, 1, &mDescriptorSetHandle);
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