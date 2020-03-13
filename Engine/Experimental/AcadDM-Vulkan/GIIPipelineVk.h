#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIShaderProgramVk;

    class GIIPipelineVk 
    {
    public:
        virtual ~GIIPipelineVk() = default;
        virtual SharedPtr<GIShaderProgramVk> GetShaderProgram() const = 0;
        
        virtual VkDescriptorSetLayout GetDescriptorSetLayout() const = 0;
        virtual VkDescriptorPool GetDescriptorPool() const = 0;
        
        virtual VkPipelineLayout GetPipelineLayout() const = 0;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;
    };
}