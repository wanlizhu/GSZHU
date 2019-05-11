#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/DESC/SGraphicsPipelineDesc.h>
#include <GSZHU/DESC/SComputePipelineDesc.h>

namespace GSZHU {
    struct SPipelineStateDesc : public SDeviceObjectAttribs {
        bool IsComputePipeline = false;
        UINT SRBAllocationGranularity = 1;
        UINT64 CommandQueueMask = 1;
        SGraphicsPipelineDesc GraphicsPipelineDesc;
        SComputePipelineDesc ComputePipelineDesc;
    };
}