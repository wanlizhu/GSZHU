#pragma once

#include <GSZHU/DESC/SSampleDesc.h>
#include <GSZHU/DESC/SBlendStateDesc.h>
#include <GSZHU/DESC/SRasterizerStateDesc.h>
#include <GSZHU/DESC/SDepthStencilStateDesc.h>
#include <GSZHU/DESC/SInputLayoutDesc.h>
#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/EPrimitiveTopology.h>
#include <GSZHU/IShader.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    // This structure describes the graphics pipeline state and is part of the PipelineStateDesc structure.
    struct GSZHU_API SGraphicsPipelineDesc {
        IShader* ShaderVS = nullptr;
        IShader* ShaderPS = nullptr;
        IShader* ShaderDS = nullptr;
        IShader* ShaderHS = nullptr;
        IShader* ShaderGS = nullptr;

        SBlendStateDesc BlendStateDesc;
        uint32_t SampleMask = 0xFFFFFFFF;
        SRasterizerStateDesc RasterizerDesc;
        SDepthStencilStateDesc DepthStencilDesc;

        SInputLayoutDesc InputLayout;
        EPRIMITIVE_TOPOLOGY PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        uint8_t NumViewports = 1;
        uint8_t NumRenderTargets = 0;
        ETEXTURE_FORMAT RTVFormatArray[8] = {};
        ETEXTURE_FORMAT DSVFormat = TEX_FORMAT_UNKNOWN;

        SSampleDesc SampleDesc;
        uint32_t NodeMask = 0;

        SGraphicsPipelineDesc() noexcept;
        bool operator==(const SGraphicsPipelineDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SGraphicsPipelineDesc> {
        size_t operator()(const GSZHU::SGraphicsPipelineDesc& Desc) const {
            return GSZHU::ComputeHash(Desc.ShaderVS,
                                      Desc.ShaderPS, 
                                      Desc.ShaderDS,
                                      Desc.ShaderHS,
                                      Desc.ShaderGS,
                                      std::hash<GSZHU::SBlendStateDesc>()(Desc.BlendStateDesc),
                                      Desc.SampleMask,
                                      std::hash<GSZHU::SRasterizerStateDesc>()(Desc.RasterizerDesc),
                                      std::hash<GSZHU::SDepthStencilStateDesc>()(Desc.DepthStencilDesc),
                                      std::hash<GSZHU::SInputLayoutDesc>()(Desc.InputLayout),
                                      static_cast<int>(Desc.PrimitiveTopology),
                                      Desc.NumViewports,
                                      Desc.NumRenderTargets,
                                      GSZHU::ComputeArrayHash<GSZHU::ETEXTURE_FORMAT>(Desc.RTVFormatArray, 8),
                                      static_cast<int>(Desc.DSVFormat),
                                      std::hash<GSZHU::SSampleDesc>()(Desc.SampleDesc),
                                      Desc.NodeMask);
        }
    };
}

