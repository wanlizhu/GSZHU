#pragma once

#include "Wanlix/CommonRHI.h"
#include "Wanlix/Object.h"

namespace Wanlix
{
    class IResourceView;

    class IResource : public Object
    {
        CLASSINFO(IResource)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual ~IResource() = 0;
        virtual ResourceType GetResourceType() const = 0;
        virtual BitSet GetBindFlags() const = 0;
        virtual Bool HasGlobalState() const = 0;
        virtual ResourceState GetGlobalState() const = 0;
        virtual ResourceState GetSubresourceState(Uint arraySlice, Uint mipLevel) const = 0;

        virtual IResourceView* GetShaderResourceView(Subresource subres = {}) = 0;
        virtual IResourceView* GetRenderTargetView(Subresource subres = {}) = 0;
        virtual IResourceView* GetDepthStencilView(Subresource subres = {}) = 0;
        virtual IResourceView* GetUnorderedAccessView(Subresource subres = {}) = 0;
        virtual void ReleaseViews() = 0;
    };

    class IResourceView : public Object
    {
        CLASSINFO(IResourceView)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        friend class IResource;
        virtual ~IResourceView() = 0;
        virtual ResourceHandle GetApiHandle() const = 0;
        virtual Subresource GetSubresource() const = 0;
    };
}