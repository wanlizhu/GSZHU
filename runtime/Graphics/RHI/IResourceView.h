#pragma once

#include "RHI/IResource.h"

namespace Wanlix
{
    struct ResourceViewDesc
    {
        const bool isbuffer;
        Subresource subresource; // for texture
        Range range; // for buffer

        ResourceViewDesc() = default;
        ResourceViewDesc(const Subresource& subres)
            : subresource(subres)
            , isbuffer(false)
        {}
        ResourceViewDesc(const Range& range)
            : range(range)
            , isbuffer(true)
        {}
        inline bool operator==(const ResourceViewDesc& rhs) const
        {
            return isbuffer ? (range == rhs.range) : (subresource == rhs.subresource);
        }
    };

    class IResourceView : public IDeviceObject
    {
    public:
        virtual const ResourceViewDesc& GetDesc() const = 0;
        virtual IResource* GetResource() const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
    };
}