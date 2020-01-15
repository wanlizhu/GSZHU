#pragma once

#include "RHI/IResource.h"
#include "RHI/IResourceView.h"

namespace Wanlix
{
    class Sampler;

    class ITexture : public IResource
    {
    public:
        virtual EPixelFormat GetFormat() const = 0;
        virtual Extent GetExtent(Uint mipLevel = 0) const = 0;
        virtual Uint GetSliceCount() const = 0;
        virtual Uint GetMipCount() const = 0;
        virtual Uint GetSampleCount() const = 0;
        virtual Uint GetSubresourceIndex(const Subresource& subres) const = 0;
        virtual void GenerateMips(IDrawContext* dc) const = 0;
        virtual const String& GetSourceFile() const = 0;
        virtual void CaptureToFile(Uint mipLevel, Uint slice, const String& filename) = 0;
        virtual SharedPtr<IResourceView> GetSRV(const Subresource& subres) = 0;
        virtual SharedPtr<IResourceView> GetRTV(const Subresource& subres) = 0;
        virtual SharedPtr<IResourceView> GetDSV(const Subresource& subres) = 0;
        virtual SharedPtr<IResourceView> GetRTV(const Subresource& subres) = 0;

        // Interfaces inherited from IResource
        virtual EResourceType GetType() const = 0;
        virtual EBindFlags GetBindFlags() const = 0;
        virtual size_t GetSizeInBytes() const = 0;
        virtual Bool HasGlobalState() const = 0;
        virtual EResourceState GetGlobalState() const = 0;
        virtual EResourceState GetSubresourceState(Subresource subres) const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
    };
}