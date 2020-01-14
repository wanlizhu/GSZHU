#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    struct Subresource
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint slice = 0;
        Uint sliceCount = 1;
        Uint mipLevel = 0;
        Uint mipLevelCount = 1;

        static const Subresource& All()
        {
            static const Subresource all(0, kMax, 0, kMax);
            return all;
        }

        Subresource() = default;
        Subresource(Uint slice, Uint mipLevel)
            : slice(slice)
            , sliceCount(1)
            , mipLevel(mipLevel)
            , mipLevelCount(1)
        {}
        Subresource(Uint slice, Uint sliceCount, Uint mipLevel, Uint mipLevelCount)
            : slice(slice)
            , sliceCount(sliceCount)
            , mipLevel(mipLevel)
            , mipLevelCount(mipLevelCount)
        {}
        inline bool operator==(const Subresource& rhs) const
        {
            return slice == rhs.slice
                && sliceCount == rhs.sliceCount 
                && mipLevel == rhs.mipLevel 
                && mipLevelCount == rhs.mipLevelCount;
        }
    };

    class IResource : public IDeviceObject
    {
    public:
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