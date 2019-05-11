#pragma once

#include <GSZHU/IDeviceObject.h>

namespace GSZHU {
    class IResourceTable : public IObject {
    public:
        struct SResourceEntry {
            const char* Name = nullptr;
            UINT Index = 0; // For array resources, index in the array
            IDeviceObject* Object = nullptr;

            SResourceEntry() = default;
            SResourceEntry(const char* _Name, IDeviceObject* _Object) noexcept
                : Name(_Name)
                , Object(_Object)
            {}
        };

        struct SResourceTableDesc {
            IResourceTable::SResourceEntry* EntryArray = nullptr;
            UINT EntryCount = 0;

            SResourceTableDesc() noexcept {}
            explicit SResourceTableDesc(IResourceTable::SResourceEntry* Entries, UINT Count) noexcept
                : EntryArray(Entries)
                , EntryCount(Count)
            {}
        };

        virtual void AddResource(const char* Name, IDeviceObject* Object, bool IsUnique) = 0;
        virtual void AddResourceArray(const char* Name, IDeviceObject** Objects, UINT Begin, UINT Count, bool IsUnique) = 0;
        virtual void RemoveResource(const char* Name, UINT Index = 0) = 0;
        virtual IDeviceObject* GetResource(const char* Name, UINT Index = 0) = 0;
    };
}