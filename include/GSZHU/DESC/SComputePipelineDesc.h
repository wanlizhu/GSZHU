#pragma once

#include <GSZHU/IShader.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SComputePipelineDesc {
        IShader* ShaderCS = nullptr;

        SComputePipelineDesc() noexcept;
        SComputePipelineDesc(IShader* _Shader) noexcept;

        bool operator==(const SComputePipelineDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SComputePipelineDesc> {
        size_t operator()(const GSZHU::SComputePipelineDesc& Desc) const {
            return std::hash<GSZHU::IShader*>()(Desc.ShaderCS);
        }
    };
}