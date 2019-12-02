#pragma once

#include "Wanlix/Core3D/Graphics/GObject.h"
#include "Wanlix/Core3D/Graphics/Types.h"
#include "Wanlix/Core3D/Graphics/Flags.h"
#include "Buffer.h"
#include "Texture.h"

namespace Wanlix
{
    class SwapChain;

    class CommandBuffer : public GObject
    {
    public:
        using Ptr      = std::shared_ptr<CommandBuffer>;
        using ConstPtr = std::shared_ptr<const CommandBuffer>;

        /* ----- Encoding ----- */

        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Execute(CommandBuffer& deferredCommandBuffer) = 0;

        /* ----- Blitting ----- */

        virtual void UpdateBuffer(
            Buffer&      dstBuffer,
            const Range& dstRange,
            const void*  srcData,
            const Range& srcRange = Range::All()
        ) = 0;

        virtual void CopyBuffer(
            Buffer&       dstBuffer,
            const Range&  dstRange,
            const Buffer& srcBuffer,
            const Range&  srcRange = Range::All()
        ) = 0;

        virtual void FillBuffer(
            Buffer& buffer,
            const Range& range,
            uint32_t value
        ) = 0;

        virtual void CopyTexture(
            Texture&               dstTexture,
            const TextureLocation& dstLocation,
            Texture&               srcTexture,
            const TextureLocation& srcLocation,
            const Extent&          extent
        ) = 0;

        virtual void CopyTexture(
            Texture& dstTexture,
            const Region& dstRegion,
            const Texture& srcTexture,
            const Region& srcRegion
        ) = 0;

        virtual void GenerateMips(Texture& texture) = 0;
        virtual void GenerateMips(Texture& texture, const TextureSubresource& subresource) = 0;

        /* ----- Viewport and Scissor ----- */

        virtual void SetViewport(const Viewport& viewport) = 0;
        virtual void SetViewports(int numViewports, const Viewport* viewports) = 0;
        virtual void SetScissor(const Scissor& scissor) = 0;
        virtual void SetScissors(int numScissors, const Scissor* scissors) = 0;

        /* ----- Clear ----- */

        virtual void SetClearColor(const Color4F& color) = 0;
        virtual void SetClearDepth(float depth) = 0;
        virtual void SetClearStencil(uint32_t stencil) = 0;
        virtual void Clear(long flags) = 0;
        virtual void ClearAttachments(int numAttachments, const AttachmentClear* attachments) = 0;

        /* ----- Input Assembly ------ */

        virtual void SetVertexBuffer(Buffer& buffer) = 0;
        virtual void SetIndexBuffer(Buffer& buffer) = 0;

        /* ----- Resources ----- */

        virtual void SetResourceHeap(
            ResourceHeap& resourceHeap,
            const PipelineBindPoint bindPoint = PipelineBindPoint::Undefined,
            std::uint32_t           firstSet = 0
        ) = 0;

        virtual void SetResource(
            Resource&       resource,
            std::uint32_t   slot,
            long            bindFlags,
            long            stageFlags = StageFlags::AllStages
        ) = 0;

        virtual void ResetResourceSlots(
            const ResourceType  resourceType,
            std::uint32_t       firstSlot,
            std::uint32_t       numSlots,
            long                bindFlags,
            long                stageFlags = StageFlags::AllStages
        ) = 0;
    };
}