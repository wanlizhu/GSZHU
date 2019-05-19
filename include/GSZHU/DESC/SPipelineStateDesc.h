#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/DESC/SGraphicsPipelineDesc.h>
#include <GSZHU/DESC/SComputePipelineDesc.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SPipelineStateDesc : public SDeviceObjectAttribs {
        bool IsComputePipeline = false;
        uint32_t SRBAllocationGranularity = 1;
        uint64_t CommandQueueMask = 1;
        SGraphicsPipelineDesc GraphicsPipelineDesc;
        SComputePipelineDesc ComputePipelineDesc;

        SPipelineStateDesc() noexcept;
        bool operator==(const SPipelineStateDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SPipelineStateDesc> {
        size_t operator()(const GSZHU::SPipelineStateDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.IsComputePipeline),
                                      Desc.SRBAllocationGranularity,
                                      Desc.CommandQueueMask,
                                      std::hash<GSZHU::SGraphicsPipelineDesc>()(Desc.GraphicsPipelineDesc),
                                      std::hash<GSZHU::SComputePipelineDesc>()(Desc.ComputePipelineDesc));
        }
    };
}