#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    /* TODO:
     * Once created, a pipeline cache can be passed to the vkCreateGraphicsPipelines and vkCreateComputePipelines commands. 
     * If the pipeline cache passed into these commands is not VK_NULL_HANDLE, 
     * the implementation will query it for possible reuse opportunities and update it with new content. 
     * The use of the pipeline cache object in these commands is internally synchronized, 
     * and the same pipeline cache object can be used in multiple threads simultaneously.
    */
    class GIPipelineCachesVk
    {
    public:

    };
}