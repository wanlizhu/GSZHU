#include <GSZHU/DESC/SPipelineStateDesc.h>

namespace GSZHU {
    SPipelineStateDesc::SPipelineStateDesc() noexcept
    {}

    bool SPipelineStateDesc::operator==(const SPipelineStateDesc& rhs) const {
        if (IsComputePipeline != rhs.IsComputePipeline
            || SRBAllocationGranularity != rhs.SRBAllocationGranularity
            || CommandQueueMask != rhs.CommandQueueMask)
            return false;

        if (IsComputePipeline)
            return ComputePipelineDesc == rhs.ComputePipelineDesc;
        else
            return GraphicsPipelineDesc == rhs.GraphicsPipelineDesc;
    }
}