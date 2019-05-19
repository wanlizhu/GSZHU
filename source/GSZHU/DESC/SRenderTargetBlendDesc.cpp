#include <GSZHU/DESC/SRenderTargetBlendDesc.h>

namespace GSZHU {
    SRenderTargetBlendDesc::SRenderTargetBlendDesc() noexcept
    {}

    SRenderTargetBlendDesc::SRenderTargetBlendDesc(bool _BlendEnable) noexcept
        : BlendEnable(_BlendEnable)
    {}

    bool SRenderTargetBlendDesc::operator==(const SRenderTargetBlendDesc& rhs) const {
        return BlendEnable == rhs.BlendEnable
            && LogicOperationEnable == rhs.LogicOperationEnable
            && SrcBlend == rhs.SrcBlend
            && DestBlend == rhs.DestBlend
            && BlendOp == rhs.BlendOp
            && SrcBlendAlpha == rhs.SrcBlendAlpha
            && DestBlendAlpha == rhs.DestBlendAlpha
            && BlendOpAlpha == rhs.BlendOpAlpha
            && LogicOp == rhs.LogicOp
            && RenderTargetWriteMask == rhs.RenderTargetWriteMask;
    }
}