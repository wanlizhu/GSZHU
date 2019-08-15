#pragma once

#include "Graphics/Viewport.h"
#include "Graphics/CopyTextureAttribs.h"
#include "Graphics/DrawAttribs.h"
#include "Graphics/DispatchComputeAttribs.h"
#include "Graphics/IDeviceObject.h"
#include "Graphics/IBuffer.h"
#include "Graphics/IShader.h"
#include "Graphics/ITexture.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/IFence.h"
#include "Graphics/ICommandList.h"
#include "Graphics/ISwapChain.h"

namespace ZHU
{
    class ZHU_API IDeviceContext : public Object
    {
    public:
        virtual void SetPipelineState() = 0;
        
        virtual void SetStencilRef(uint32_t ref) = 0;
        virtual void SetBlendState() = 0;
        virtual void SetVertexBuffer() = 0;
        virtual void SetIndexBuffer() = 0;
        virtual void SetViewports() = 0;
        virtual void SetScissors() = 0;
        virtual void SetRenderTarget() = 0;
        virtual void Draw() = 0;
        virtual void DispatchCompute() = 0;
        virtual void ClearDepthStencil() = 0;
        virtual void ClearRenderTarget() = 0;
        virtual void InvalidateState() = 0;

        virtual void FinishCommandList() = 0;
        virtual void ExecuteCommandList() = 0;
        virtual void SignalFence() = 0;
        virtual void Flush() = 0;

        virtual void UpdateBuffer() = 0;
        virtual void CopyBuffer() = 0;
        virtual void MapBuffer() = 0;
        virtual void UnmapBuffer() = 0;

        virtual void UpdateTexture() = 0;
        virtual void CopyTexture() = 0;
        virtual void MapTexture() = 0;
        virtual void UnmapTexture() = 0;

        virtual void GenerateMips() = 0;
        virtual void SetSwapChain() = 0;
        virtual void FinishFrame() = 0;

        virtual void TransitionShaderResources() = 0;
        virtual void CommitShaderResources() = 0;
        virtual void TransitionResourceStates() = 0;
    };
}