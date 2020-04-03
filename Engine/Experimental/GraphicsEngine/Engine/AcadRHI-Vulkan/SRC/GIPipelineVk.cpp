#include "GIPipelineVk.h"
#include "GIDeviceVk.h"
#include "GIPipelineLayoutVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "GIShaderReflectionVk.h"
#include "GIVertexLayoutVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIPipelineVk::GIPipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<GIShaderReflectionVk> reflection,
        const VkGraphicsPipelineCreateInfo& createInfo,
        VkPipelineCache cache
    )
        : GIDeviceObjectVk(device)
        , mShaderReflection(reflection)
    {
        CreatePipelineLayout();
        VK_CHECK(vkCreateGraphicsPipelines(*mDevice, cache, 1, &createInfo, nullptr, &mPipelineHandle));
    }

    GIPipelineVk::~GIPipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipelineHandle, nullptr);
            mPipelineHandle = VK_NULL_HANDLE;
        }
    }

    void GIPipelineVk::CreatePipelineLayout()
    {
        std::vector<SharedPtr<GIDescriptorSetLayoutVk>> setLayouts;
        std::vector<VkPushConstantRange> pushConstants = mShaderReflection->GetPushConstantRanges();

        for (const auto& [setId, setBindings] : mShaderReflection->GetDescriptorSetLayoutInfos())
        {
            auto setLayout = GIDescriptorSetLayoutVk::Create(
                mDevice,
                mShaderReflection,
                setBindings,
                mShaderReflection->IsPushDescriptorSet(setId)
            );
            setLayouts.push_back(setLayout);
        }

        mPipelineLayout = GIPipelineLayoutVk::Create(mDevice, setLayouts, pushConstants);
    }

    GIPipelineVk::operator const VkPipeline& () const
    {
        return mPipelineHandle;
    }

    bool GIPipelineVk::IsValid() const
    {
        return mPipelineHandle != VK_NULL_HANDLE;
    }

    void GIPipelineVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipelineHandle, 
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            name
        );
    }

    void GIPipelineVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipelineHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            tag
        );
    }

    void GIPipelineVk::SetPipelineName(const std::wstring& name)
    {
        mPipelineName = name;
    }

    const std::wstring& GIPipelineVk::GetPipelineName() const
    {
        return mPipelineName;
    }

    SharedPtr<GIPipelineDynamicStateVk> GIPipelineVk::GetDynamicState(VkDynamicState state) const
    {
        auto it = mDynamicStates.find(state);
        if (it == mDynamicStates.end())
            return nullptr;
        else
            return it->second;
    }

    SharedPtr<GIShaderReflectionVk> GIPipelineVk::GetShaderReflection() const
    {
        return mShaderReflection;
    }

    SharedPtr<GIPipelineLayoutVk> GIPipelineVk::GetPipelineLayout() const
    {
        return mPipelineLayout;
    }

    VkPipelineBindPoint GIPipelineVk::GetPipelineBindPoint() const
    {
        return mPipelineBindPoint;
    }
}