#pragma once

#include "GIDeviceObject.h"

namespace GS
{
    enum class GIResourceType
    {
        Undefined = 0,
        Buffer,
        Texture1D,
        Texture2D,
        Texture3D,
        TextureCube,
        Texture2DMS, // 2D multi-sampled texture
    };

    class GIResource : public GIDeviceObject
    {
    public:
        using Ptr = std::shared_ptr<GIResource>;
        using ConstPtr = std::shared_ptr<const GIResource>;

        virtual void Flush();
        virtual void Flush(const GIRange& range);
        virtual void Flush(const GIRegion& region);

        virtual void Update(const void* data, bool withFlush = false);
        virtual void Update(const GIRange& range, const void* data, bool withFlush = false);
        virtual void Update(const GIRegion& region, const void* data, bool withFlush = false);

        virtual size_t Read(void* data) const;
        virtual size_t Read(const GIRange& range, void* data) const;
        virtual size_t Read(const GIRegion& region, void* data) const;
        
        inline GIResourceType GetType() const { return mType; }
        inline const GIRange& GetRange() const { return std::get<GIRange>(mShape); }
        inline const GIRegion& GetRegion() const { return std::get<GIRegion>(mShape); }
        inline size_t GetNumBytes() const { return mNumBytes; }

    protected:
        GIResource(const std::string& name, 
                   GIDevice::Ptr device,
                   GIResourceType type,
                   GIRegion region,
                   size_t bytes);

    protected:
        GIResourceType mType = GIResourceType::Undefined;
        std::variant<GIRange, GIRegion> mShape;
        size_t mNumBytes = 0;
    };
}