#include <GSZHU/DESC/SGraphicsPipelineDesc.h>

namespace GSZHU {
    SGraphicsPipelineDesc::SGraphicsPipelineDesc() noexcept
    {}

    bool SGraphicsPipelineDesc::operator==(const SGraphicsPipelineDesc& rhs) const {
        return ShaderVS == rhs.ShaderVS
            && ShaderPS == rhs.ShaderPS
            && ShaderDS == rhs.ShaderDS
            && ShaderHS == rhs.ShaderHS
            && ShaderGS == rhs.ShaderGS
            && BlendStateDesc == rhs.BlendStateDesc
            && SampleMask == rhs.SampleMask
            && RasterizerDesc == rhs.RasterizerDesc
            && DepthStencilDesc == rhs.DepthStencilDesc
            && InputLayout == rhs.InputLayout
            && PrimitiveTopology == rhs.PrimitiveTopology
            && NumViewports == rhs.NumViewports
            && NumRenderTargets == rhs.NumRenderTargets
            && RTVFormatArray[0] == rhs.RTVFormatArray[0]
            && RTVFormatArray[1] == rhs.RTVFormatArray[1]
            && RTVFormatArray[2] == rhs.RTVFormatArray[2]
            && RTVFormatArray[3] == rhs.RTVFormatArray[3]
            && RTVFormatArray[4] == rhs.RTVFormatArray[4]
            && RTVFormatArray[5] == rhs.RTVFormatArray[5]
            && RTVFormatArray[6] == rhs.RTVFormatArray[6]
            && RTVFormatArray[7] == rhs.RTVFormatArray[7]
            && DSVFormat == rhs.DSVFormat
            && SampleDesc == rhs.SampleDesc
            && NodeMask == rhs.NodeMask;
    }
}