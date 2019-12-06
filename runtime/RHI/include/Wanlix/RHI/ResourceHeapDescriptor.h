#pragma once

#include "Buffer.h"
#include "Texture.h"

namespace Wanlix
{
    class PipelineLayout;

    struct ResourceViewDescriptor
    {
        Resource* resource = nullptr;
        ResourceViewDescriptor* view = nullptr;
    };

    struct ResourceHeapDescriptor
    {
        PipelineLayout* pipelineLayout = nullptr;
        std::vector<ResourceViewDescriptor> resourceViews;
    };
}