#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVShaderProgram;

    class GIIPipelineVk 
    {
    public:
        virtual ~GIIPipelineVk() = default;

        // Query shader resource variable from it by name
        virtual SharedPtr<SPIRVShaderProgram> GetShaderProgram() const = 0;

        // The pipeline name could be used for searching pipeline cache
        virtual void SetPipelineName(const std::wstring& name) = 0;
        virtual const std::wstring& GetPipelineName() const = 0;
        
        // One pipeline layout could contain multiple descriptor-set-layouts 
        // Use vkCmdPushDescriptorSetKHR for frequenct updated resources
        virtual std::vector<uint32_t> GetDescriptorSetLayoutIndices() const = 0;
        virtual bool IsPushDescriptorSet(uint32_t setIndex) const = 0;
        virtual std::optional<VkDescriptorSetLayout> GetDescriptorSetLayout(uint32_t setIndex) const = 0;
        virtual VkDescriptorPool GetDescriptorPool() = 0; // return a "thread_local" pool
        
        // Pipeline layout is required for binding/pushing descriptor-set and pushing constants
        virtual VkPipelineLayout GetPipelineLayout() const = 0;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;
    };
}