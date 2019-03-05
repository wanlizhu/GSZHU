#pragma once

#include <NTGS/Graphics/IDeviceObject.h>

namespace NTGS {
    struct ResourceMappingEntry {
        const char* Name = nullptr;
        IDeviceObject* Object = nullptr;
        UINT ArrayIndex = 0;

        ResourceMappingEntry() = default;
        ResourceMappingEntry(const char* _Name, IDeviceObject* _Object, UINT _ArrayIndex = 0) noexcept 
            : Name(_Name),
              Object(_Object),
              ArrayIndex(_ArrayIndex) {}
    };

    struct ResourceMappingDesc {
        ResourceMappingEntry* Entries = nullptr;

        ResourceMappingDesc() = default;
        explicit ResourceMappingDesc(ResourceMappingEntry* _Entries) noexcept
            : Entries(_Entries) {}
    };

    class IResourceMapping : public IObject
    {
    public:
        virtual void AddResource(const char* Name, IDeviceObject* Object, bool IsUnique) = 0;
        virtual void AddResourceArray(const char* Name, UINT StartIndex, IDeviceObject* const* Objects, UINT NumElements, bool IsUnique) = 0;
        virtual void RemoveResourceByName(const char* Name, UINT ArrayIndex = 0) = 0;
        virtual void GetResource(const char* Name, IDeviceObject** ppResource, UINT ArrayIndex = 0) = 0;
        virtual SIZE GetSize() = 0;
    };
}