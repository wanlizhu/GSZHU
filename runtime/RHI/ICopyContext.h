#pragma once

#include "RHI/IDeviceContext.h"

namespace Wanlix
{
    class ICopyContext : public IDeviceContext
    {
    public:
        virtual void CopyTexture(const ITexture* src, const ITexture* dst) = 0;
        virtual void CopySubresource(const ITexture* src, Uint srcSubIndex, const Region& srcSubRegion, const ITexture* dst, Uint dstSubIndex, const Offset& dstSubOffset) = 0;
        virtual void CopyBuffer(const IBuffer* src, const ITexture* dst) = 0;
        virtual void CopyRange(const IBuffer* src, const Range& srcRange, const IBuffer* dst, Uint dstOffset) = 0;
        virtual void UpdateTexture(const ITexture* texture, const void* data) = 0;
        virtual void UpdateSubresource(const ITexture* texture, Uint subIndex, const Region& region, const void* data) = 0;
        virtual void UpdateBuffer(const IBuffer* buffer, const void* data) = 0;
        virtual void UpdateRange(const IBuffer* buffer, const Range& range, const void* data) = 0;
        virtual ByteArrayPtr ReadTexture(const ITexture* texture) = 0;
        virtual ByteArrayPtr ReadSubresource(const ITexture* texture, Uint subres, const Region& region) = 0;
        virtual ByteArrayPtr ReadBuffer(const IBuffer* buffer) = 0;
        virtual ByteArrayPtr ReadRange(const IBuffer* buffer, const Range& range) = 0;
        virtual AsyncData ReadTextureAsync(const ITexture* texture) = 0;
        virtual AsyncData ReadSubresourceAsync(const ITexture* texture, Uint subres, const Region& region) = 0;
        virtual AsyncData ReadBufferAsync(const IBuffer* buffer) = 0;
        virtual AsyncData ReadRangeAsync(const IBuffer* buffer, const Range& range) = 0;
    
        // Interfaces inherited from IDeviceContext
        virtual void Flush(bool wait = false) = 0;
        virtual bool HasPendingCommands() const = 0;
        virtual bool SetPendingCommands(bool flag) const = 0;
        virtual bool AddResourceBarrier(const IResourceView& view, EResourceState newState) = 0;
        virtual Handle GetCommandBuffer() const = 0;
        virtual const DeviceContextData& GetDeviceContextData() const = 0;
        virtual void SetDeviceContextData(const DeviceContextData& data) = 0;
        virtual void BindDescriptorHeaps() = 0;
    };
}