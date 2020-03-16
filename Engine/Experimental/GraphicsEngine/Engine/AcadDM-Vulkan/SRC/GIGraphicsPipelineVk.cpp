#include "GIGraphicsPipelineVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIGraphicsPipelineVk::GIGraphicsPipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<SPIRVShaderProgram> program,
        const VkGraphicsPipelineCreateInfo& createInfo)
        : GIDeviceObjectVk(device)
        , mShaderProgram(program)
    {
        
    }

    GIGraphicsPipelineVk::~GIGraphicsPipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipeline, nullptr);
            mPipeline = VK_NULL_HANDLE;
        }
    }

    bool GIGraphicsPipelineVk::IsValid() const
    {

    }

    void GIGraphicsPipelineVk::SetDebugName(const char* name) const override final;
    void GIGraphicsPipelineVk::SetDebugTag(const DebugTag& tag) const override final;
    SharedPtr<SPIRVShaderProgram> GIGraphicsPipelineVk::GetShaderProgram() const override final;
    VkDescriptorSetLayout GIGraphicsPipelineVk::GetDescriptorSetLayout() const override final;
    VkDescriptorPool GIGraphicsPipelineVk::GetDescriptorPool() const override final;
    VkPipelineLayout GIGraphicsPipelineVk::GetPipelineLayout() const override final;
    VkPipelineBindPoint GIGraphicsPipelineVk::GetPipelineBindPoint() const override final;

    
}