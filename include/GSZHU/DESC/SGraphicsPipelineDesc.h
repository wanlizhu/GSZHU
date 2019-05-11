#pragma once

#include <GSZHU/DESC/SSampleDesc.h>
#include <GSZHU/DESC/SBlendStateDesc.h>
#include <GSZHU/DESC/SRasterizerStateDesc.h>
#include <GSZHU/DESC/SDepthStencilStateDesc.h>
#include <GSZHU/DESC/SInputLayoutDesc.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/EPrimitiveTopology.h>

namespace GSZHU {
    // This structure describes the graphics pipeline state and is part of the PipelineStateDesc structure.
    struct SGraphicsPipelineDesc {
        IShader* ShaderVS = nullptr;
        IShader* ShaderPS = nullptr;
        IShader* ShaderDS = nullptr;
        IShader* ShaderHS = nullptr;
        IShader* ShaderGS = nullptr;

        SBlendStateDesc BlendStateDesc;
        UINT SampleMask = 0xFFFFFFFF;
        SRasterizerStateDesc RasterizerDesc;
        SDepthStencilStateDesc DepthStencilDesc;

        SInputLayoutDesc InputLayout;
        EPRIMITIVE_TOPOLOGY PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        UINT8 NumViewports = 1;
        UINT8 NumRenderTargets = 0;
        ETEXTURE_FORMAT RTVFormats[8] = {};
        ETEXTURE_FORMAT DSVFormat = TEX_FORMAT_UNKNOWN;

        SSampleDesc SmplDesc;
        UINT NodeMask = 0;
    };
}