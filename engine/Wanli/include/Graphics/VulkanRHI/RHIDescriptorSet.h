#pragma once

#include "BasicTypes.h"
#include "Graphics/VulkanRHI/RHIUtilities.h"
#include "Graphics/VulkanRHI/RHIDeviceObject.h"

namespace Wanli
{
    class RHIPipeline;

    class DLLDECL RHIDescriptorSet : public RHIDeviceObject
    {
    public:
        RHIDescriptorSet(
            RHIDevice* device,
            RHIPipeline* pipeline
        );
        virtual ~RHIDescriptorSet();

        virtual void Destroy() override;
        inline VkDescriptorSet GetHandle() const { return mDescriptorSet; }
        inline VkDescriptorPool GetDescriptorPool() const { return mDescriptorPool; }
        inline const VkPipelineLayout& GetPipelineLayout() const { return mPipelineLayout; }
        inline VkPipelineBindPoint GetPipelineBindPoint() const { return mPipelineBindPoint; }

    protected:
        VkDescriptorSet mDescriptorSet = VK_NULL_HANDLE;
        VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
        VkPipelineLayout mPipelineLayout;
        VkPipelineBindPoint mPipelineBindPoint;
    };
}