#pragma once

#include <GSZHU/IDeviceObject.h>

namespace GSZHU {
    class GSZHU_API IResourceTable : public IObject {
    public:
        struct SResourceEntry {
            const char* Name = nullptr;
            uint32_t Index = 0; // For array resources, index in the array
            IDeviceObject* Object = nullptr;

            SResourceEntry() = default;
            SResourceEntry(const char* _Name, IDeviceObject* _Object) noexcept
                : Name(_Name)
                , Object(_Object)
            {}
        };

        struct SResourceTableDesc {
            IResourceTable::SResourceEntry* EntryArray = nullptr;
            uint32_t EntryCount = 0;

            SResourceTableDesc() noexcept {}
            explicit SResourceTableDesc(IResourceTable::SResourceEntry* Entries, uint32_t Count) noexcept
                : EntryArray(Entries)
                , EntryCount(Count)
            {}
        };

        virtual void AddResource(const char* Name, IDeviceObject* Object, bool IsUnique) = 0;
        virtual void AddResourceArray(const char* Name, IDeviceObject** Objects, uint32_t Begin, uint32_t Count, bool IsUnique) = 0;
        virtual void RemoveResource(const char* Name, uint32_t Index = 0) = 0;
        virtual IDeviceObject* GetResource(const char* Name, uint32_t Index = 0) = 0;
    };
}