#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIShaderReflectionVk;
    class GIShaderReflectionBuilderVk;
    class GIDescriptorSetLayoutVk;
    class GIPipelineLayoutVk;
    class GIPipelineDynamicStateVk;
    class GIVertexLayoutVk;

    class GIPipelineVk : public GIDeviceObjectVk
    {
        friend class GIPipelineBuilderVk;
        DECL_DEVICE_OBJECT(GIPipelineVk)
    public:
        virtual ~GIPipelineVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        
        operator const VkPipeline& () const;
        void SetPipelineName(const std::wstring& name);
        const std::wstring& GetPipelineName() const;
        SharedPtr<GIPipelineDynamicStateVk> GetDynamicState(VkDynamicState state) const;
        SharedPtr<GIShaderReflectionVk> GetShaderReflection() const;
        SharedPtr<GIPipelineLayoutVk> GetPipelineLayout() const;
        VkPipelineBindPoint GetPipelineBindPoint() const;

    protected:
        GIPipelineVk(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<GIShaderReflectionVk> reflection,
            const VkGraphicsPipelineCreateInfo& createInfo,
            VkPipelineCache cache
        );

        void CreatePipelineLayout();

    private:
        std::wstring mPipelineName;
        VkPipeline mPipelineHandle = VK_NULL_HANDLE;
        VkPipelineBindPoint mPipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        SharedPtr<GIPipelineLayoutVk> mPipelineLayout;
        SharedPtr<GIShaderReflectionVk> mShaderReflection;
        std::unordered_map<VkDynamicState, SharedPtr<GIPipelineDynamicStateVk>> mDynamicStates;
    };
}