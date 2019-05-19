#include <GSZHU/DESC/SRasterizerStateDesc.h>

namespace GSZHU {
    SRasterizerStateDesc::SRasterizerStateDesc() noexcept
    {}

    SRasterizerStateDesc::SRasterizerStateDesc(EFILL_MODE _FillMode, ECULL_MODE _CullMode) noexcept
        : FillMode(_FillMode)
        , CullMode(_CullMode)
    {}

    bool SRasterizerStateDesc::operator==(const SRasterizerStateDesc& rhs) const {
        return FillMode == rhs.FillMode             
            && CullMode == rhs.CullMode            
            && FrontCounterClockwise == rhs.FrontCounterClockwise
            && DepthBias == rhs.DepthBias        
            && DepthBiasClamp == rhs.DepthBiasClamp    
            && SlopeScaledDepthBias == rhs.SlopeScaledDepthBias
            && DepthClipEnable == rhs.DepthClipEnable 
            && ScissorEnable == rhs.ScissorEnable     
            && AntialiasedLineEnable == rhs.AntialiasedLineEnable;
    }
}