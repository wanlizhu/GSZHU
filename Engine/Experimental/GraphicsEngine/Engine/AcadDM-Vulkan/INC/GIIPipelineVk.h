#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVReflection;

    class GIIPipelineVk 
    {
    public:
        virtual ~GIIPipelineVk() = default;

        // Query shader resource variable from it by name
        virtual SharedPtr<SPIRVReflection> GetShaderReflection() const = 0;

        // The pipeline name could be used for searching pipeline cache
        virtual void SetPipelineName(const std::wstring& name) = 0;
        virtual const std::wstring& GetPipelineName() const = 0;
        
        // One pipeline layout could contain multiple descriptor-set-layouts 
        // Push descriptors are mainly intended for texture/sampler/image bindings, which don't have a convenient mechanism 
        // like dynamic offsets that would allow you to quickly switch which image/sampler you're using. 
        // That being said, push descriptors are a useful concept even for UBO/SSBOs. 
        // Whether they are faster than dynamic descriptors is a matter of profiling.
        virtual std::vector<uint32_t> GetDescriptorSetLayoutIndices() const = 0;
        virtual bool IsPushDescriptorSet(uint32_t setId) const = 0;
        virtual std::optional<VkDescriptorSetLayout> GetDescriptorSetLayout(uint32_t setId) const = 0;
        virtual VkDescriptorPool GetDescriptorPool() = 0; // return a "thread_local" pool
        
        // Pipeline layout is required for binding/pushing descriptor-set and pushing constants
        virtual VkPipelineLayout GetPipelineLayout() const = 0;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;
    };
}