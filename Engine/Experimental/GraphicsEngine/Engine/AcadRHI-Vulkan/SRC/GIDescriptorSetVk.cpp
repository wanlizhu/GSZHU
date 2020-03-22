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

        VK_CHECK(vkAllocateDescriptorSets(*mDevice, &allocInfo, &mDescriptorSet));
    }

    GIDescriptorSetVk::~GIDescriptorSetVk()
    {
        auto pool = mDescriptorPool.lock();
        if (IsValid() && pool)
        {
            vkFreeDescriptorSets(*mDevice, *pool, 1, &mDescriptorSet);
            mDescriptorSet = VK_NULL_HANDLE;
        }
    }

    bool GIDescriptorSetVk::IsValid() const
    {
        return mDescriptorSet != VK_NULL_HANDLE;
    }

    void GIDescriptorSetVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mDescriptorSet,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
            name);
    }

    void GIDescriptorSetVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mDescriptorSet,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
            tag);
    }

    GIDescriptorSetVk::operator const VkDescriptorSet& () const
    {
        return mDescriptorSet;
    }

    bool GIDescriptorSetVk::IsPushDescriptorSet() const
    {
        return mDescriptorSetLayout->IsPushDescriptorSet();
    }

    SharedPtr<GIDescriptorSetLayoutVk> GIDescriptorSetVk::GetDescriptorSetLayout() const
    {
        return mDescriptorSetLayout;
    }

    void GIDescriptorSetVk::BindResource(uint32_t binding, WeakPtr<GIDescriptorResourceVk> resource)
    {
        if (!mDescriptorSetLayout->GetDescriptorType(binding).has_value() ||
            resource.expired())
        {
            LOG_WARNING("Failed to bind resource to (%d).\n", binding);
            return;
        }

        // TODO    
    }

    void GIDescriptorSetVk::BindResourceRange(uint32_t binding, WeakPtr<GIDescriptorResourceVk> resource, std::optional<GIRange> range)
    {
        if (!mDescriptorSetLayout->GetDescriptorType(binding).has_value() ||
            resource.expired())
        {
            LOG_WARNING("Failed to bind resource to (%d).\n", binding);
            return;
        }

        // TODO
    }

    void GIDescriptorSetVk::BindResourceRegion(uint32_t binding, WeakPtr<GIDescriptorResourceVk> resource, std::optional<GIRegion> region)
    {
        if (!mDescriptorSetLayout->GetDescriptorType(binding).has_value() ||
            resource.expired())
        {
            LOG_WARNING("Failed to bind resource to (%d).\n", binding);
            return;
        }

        // TODO
    }

    void GIDescriptorSetVk::SetParentDescriptorSet(WeakPtr<GIDescriptorSetVk> set)
    {
        mParent = set;
    }

    void GIDescriptorSetVk::Update()
    {
        // TODO
    }
}