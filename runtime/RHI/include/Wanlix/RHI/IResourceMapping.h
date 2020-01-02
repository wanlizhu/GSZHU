#pragma once

#include "Wanlix/RHI/Common.h"

namespace Wanlix
{
    struct ResourceMappingEntry
    {
        String name;
        IDeviceObject* object = nullptr;
        Uint arrayIndex = 0;
    };

    struct ResourceMappingDesc
    {
        Array<ResourceMappingEntry> entries;
    };

    class RHI_API IResourceMapping : public Object
    {
        CLASSINFO(IResourceMapping)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual ~IResourceMapping() = default;

        virtual void AddResource(String const& name, IDeviceObject* object, Bool isUnique) = 0;
        virtual void AddResourceArray(String const& name, Uint startIndex, IDeviceObject** objects, Uint elements, Bool isUnique) = 0;
        virtual void RemoveResourceByName(String const& name, Uint arrayIndex) = 0;
        virtual IDeviceObject* GetResource(String const& name, Uint arrayIndex = 0) = 0;
        virtual Uint GetSize() const = 0;
    };
}