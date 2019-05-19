#include <GSZHU/DESC/SComputePipelineDesc.h>

namespace GSZHU {
    SComputePipelineDesc::SComputePipelineDesc() noexcept
    {}
       
    SComputePipelineDesc::SComputePipelineDesc(IShader* _Shader) noexcept
        : ShaderCS(_Shader)
    {}

    bool SComputePipelineDesc::operator==(const SComputePipelineDesc& rhs) const {
        return ShaderCS == rhs.ShaderCS;
    }
}