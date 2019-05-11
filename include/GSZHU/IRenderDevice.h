#pragma once

#include <GSZHU/SDeviceCaps.h>
#include <GSZHU/Constants.h>
#include <GSZHU/IBuffer.h>
#include <GSZHU/DESC/SInputLayoutDesc.h>
#include <GSZHU/IShader.h>
#include <GSZHU/ITexture.h>
#include <GSZHU/ITextureView.h>
#include <GSZHU/ISampler.h>
#include <GSZHU/IResourceTable.h>
#include <GSZHU/IBufferView.h>
#include <GSZHU/IPipelineState.h>
#include <GSZHU/IFence.h>
#include <GSZHU/DESC/SDepthStencilStateDesc.h>
#include <GSZHU/DESC/SRasterizerStateDesc.h>
#include <GSZHU/DESC/SBlendStateDesc.h>
#include <GSZHU/SBufferData.h>
#include <GSZHU/SShaderCreationAttribs.h>
#include <GSZHU/STextureFormatInfo.h>

namespace GSZHU {
    class IRenderDevice : public IObject {
    public:
        virtual IBuffer* CreateBuffer(const SBufferDesc& BufferDesc, const SBufferData* BufferData) = 0;
        virtual IShader* CreateShader(const SShaderCreationAttribs& CreationAttribs) = 0;
        virtual ITexture* CreateTexture(const STextureDesc& TextureDesc, const STextureData* Data) = 0;
        virtual ISampler* CreateSampler(const SSamplerDesc& SamplerDesc) = 0;
        virtual IResourceTable* CreateResourceTable(const IResourceTable::SResourceTableDesc& TableDesc) = 0;
        virtual IPipelineState* CreatePipelineState(const SPipelineStateDesc& PipelineStateDesc) = 0;
        virtual IFence* CreateFence(const SFenceDesc& FenceDesc) = 0;

        virtual const SDeviceCaps& GetDeviceCaps()const = 0;
        virtual const STextureFormatInfo& GetTextureFormatInfo(ETEXTURE_FORMAT TextureFormat) = 0;
        virtual const STextureFormatInfoEXT& GetTextureFormatInfoEXT(ETEXTURE_FORMAT TextureFormat) = 0;
        virtual void ReleaseStaleResources(bool ForceRelease = false) = 0; // is automatically called by ISwapChain::Present().
    };
}