#include <GSZHU/DESC/SBlendStateDesc.h>

namespace GSZHU {
    SBlendStateDesc::SBlendStateDesc() noexcept
    {}

    SBlendStateDesc::SBlendStateDesc(bool _AlphaToCoverageEnable, 
                                     bool _IndependentBlendEnable, 
                                     const SRenderTargetBlendDesc& RT0) noexcept
        : AlphaToCoverageEnable(_AlphaToCoverageEnable)
        , IndependentBlendEnable(_IndependentBlendEnable)
        , RenderTargetArray{RT0}
    {}

    bool SBlendStateDesc::operator==(const SBlendStateDesc& rhs) const {
        bool IsTargetsEqual = true;
        for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
            if (!(RenderTargetArray[i] == rhs.RenderTargetArray[i])) {
                IsTargetsEqual = false;
                break;
            }

        return IsTargetsEqual
            && AlphaToCoverageEnable == rhs.AlphaToCoverageEnable
            && IndependentBlendEnable == rhs.IndependentBlendEnable;
    }
}