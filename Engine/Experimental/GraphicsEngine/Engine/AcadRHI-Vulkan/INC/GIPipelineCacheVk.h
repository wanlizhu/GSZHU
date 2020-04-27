#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    /* TODO:
     * Once created, a pipeline cache can be passed to the vkCreateGraphicsPipelines and vkCreateComputePipelines commands. 
     * If the pipeline cache passed into these commands is not VK_NULL_HANDLE, 
     * the implementation will query it for possible reuse opportunities and update it with new content. 
     * The use of the pipeline cache object in these commands is internally synchronized, 
     * and the same pipeline cache object can be used in multiple threads simultaneously.
    */
    class GIPipelineCacheVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIPipelineCacheVk)
    public:
        static SharedPtr<GIPipelineCacheVk> Create(
            SharedPtr<GIDeviceVk> device,
            const std::wstring& path
        );
        
        virtual ~GIPipelineCacheVk();
        operator const VkPipelineCache& () const;
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        void Flush() const;

    private:
        GIPipelineCacheVk(SharedPtr<GIDeviceVk> device);

    private:
        VkPipelineCache mPipelineCacheHandle = VK_NULL_HANDLE;
        std::filesystem::path mFilePath;
    };
}