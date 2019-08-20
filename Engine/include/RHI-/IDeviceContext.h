#pragma once

#include "Graphics/RHI/Viewport.h"
#include "Graphics/RHI/IDeviceObject.h"
#include "Graphics/RHI/IBuffer.h"
#include "Graphics/RHI/IPipelineState.h"
#include "Graphics/RHI/ITexture.h"
#include "Graphics/RHI/IFence.h"
#include "Graphics/RHI/ICommandList.h"
#include "Graphics/RHI/ISwapChain.h"
#include "Graphics/RHI/IDeviceContextArgs.h"

namespace ZHU
{
    class ZHU_GS_API IDeviceContext : public Object
    {
    public:
        virtual void SetPipelineState(IPipelineState::Pointer PSO) = 0;
        
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
    };
}