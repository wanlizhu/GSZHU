#pragma once

#include "GIIPipelineVk.h"
#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIComputePipelineVk
        : public GIDeviceObjectVk
        , public GIIPipelineVk
    {
        friend class GIComputePipelineBuilderVk;
        DECL_DEVICE_OBJECT(GIComputePipelineVk)
    public:
        virtual ~GIComputePipelineVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

    protected:
        GIComputePipelineVk(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<SPIRVShaderProgram> program,
            const VkComputePipelineCreateInfo& createInfo);
        
        GIComputePipelineVk(const GIComputePipelineVk&) = delete;
        GIComputePipelineVk(GIComputePipelineVk&&) = default;
        GIComputePipelineVk& operator=(const GIComputePipelineVk&) = delete;
        GIComputePipelineVk& operator=(GIComputePipelineVk&&) = default;

    private:
        VkPipeline mPipeline = VK_NULL_HANDLE;
        SharedPtr<SPIRVShaderProgram> mShaderProgram;
    };

    class GIComputePipelineBuilderVk
    {
    public:
        GIComputePipelineBuilderVk(SharedPtr<GIDeviceVk> device);
        
        GIComputePipelineBuilderVk& SetBasePipeline(SharedPtr<GIComputePipelineVk> pipeline);
        GIComputePipelineBuilderVk& SetShaderProgram(SharedPtr<SPIRVShaderProgram> shaderProgram);
        
        SharedPtr<GIComputePipelineVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        SharedPtr<SPIRVShaderProgram> mShaderProgram;
        VkComputePipelineCreateInfo mCreateInfo = {};
    };
}