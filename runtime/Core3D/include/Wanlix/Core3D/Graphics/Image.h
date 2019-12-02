#pragma once

#include "Types.h"

namespace Wanlix
{
    struct DestImageDescriptor
    {
        ColorFormat colorFormat = ColorFormat::RGBA;
        DataType    dataType = DataType::UInt8;
        size_t      dataSize = 0;
        void* data = nullptr;
    };

    struct SourceImageDescriptor
    {
        ColorFormat colorFormat = ColorFormat::RGBA;
        DataType    dataType = DataType::UInt8;
        size_t      dataSize = 0;
        const void* data = nullptr;

        SourceImageDescriptor() = default;
        SourceImageDescriptor(const DestImageDescriptor& rhs)
            : colorFormat(rhs.colorFormat)
            , dataType(rhs.dataType)
            , dataSize(rhs.dataSize)
            , data(rhs.data)
        {}
    };

    class Image
    {
    public:
        using Ptr       = std::shared_ptr<Image>;
        using ConstPtr  = std::shared_ptr<const Image>;
        using SourceDesc  = SourceImageDescriptor;
        using DestDesc = DestImageDescriptor;

        Image() noexcept = default;
        explicit Image(
            const Extent&      extent,
            const ColorFormat& format = ColorFormat::RGBA,
            const DataType&    dataType = DataType::UInt8,
            ByteBuffer&&       data = nullptr,
            const Color4F&     fillColor = Color4F::Black()
        ) noexcept;
        Image(const Image& rhs) noexcept;
        Image(Image&& rhs) noexcept;

        Image& operator=(const Image& rhs);
        Image& operator=(Image&& rhs);


        /* ----- Storage ----- */

        void Convert(const ColorFormat& format, const DataType& dataType, int threads = 0);
        //void Resize(const Extent& extent, const SamplerFilter& filter);
        void Resize(const Extent& extent, const Color4F& fillColor);
        void Swap(Image& rhs);
        void Reset();
        ByteBuffer Detach();

        /* ----- Pixels ----- */

        void CopyTo(const Image& dstImage, const Region& dstRegion);
        void ReadPixels(const Region& region, const DestDesc& dstImageDesc) const;
        void WritePixels(const Region& region, const SourceDesc& srcImageDesc);
        void Fill(const Region& region, const Color4F& fillColor);
        void MirrorYZ();
        void MirrorXZ();
        void MirrorXY();

        /* ----- Attributes ----- */

        SourceDesc GetDescriptor() const;
        DestDesc   GetDescriptor();
        uint32_t   GetBytesPerPixel() const;
        uint32_t   GetBytesPerRow() const;
        uint32_t   GetDataSize() const;
        uint32_t   GetNumPixels() const;
        bool       IsRegionInside(const Region& region) const;

        Region             GetRegion() const    { return Region(Offset(0, 0, 0), mExtent); }
        const Extent&      GetExtent() const    { return mExtent; }
        const ColorFormat& GetFormat() const    { return mFormat; }
        const DataType&    GetDataType() const  { return mDataType; }
        const void*        GetData() const      { return mData.get(); }
        void*              GetData()            { return mData.get(); }

    protected:
        void   ResetAttributes();
        Region ClampRegion(const Region& region) const;
        size_t GetDataAt(const Offset& offset) const;

    protected:
        ColorFormat mFormat = ColorFormat::RGBA;
        DataType    mDataType = DataType::UInt8;
        Extent      mExtent;
        ByteBuffer  mData;
    };
}