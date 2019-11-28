#pragma once

#include "ImageDescriptor.h"
#include <memory>

namespace Wanlix
{
    class Image
    {
    public:
        using Ptr = std::shared_ptr<Image>;
        using ConstPtr = std::shared_ptr<const Image>;

        Image() noexcept = default;
        explicit Image(
            const Extent& extent,
            const ImageFormat& format = ImageFormat::RGBA,
            const DataType& dataType = DataType::UInt8,
            const ByteBuffer& data = nullptr
        ) noexcept;
        Image(const Image& rhs) noexcept;
        Image(Image&& rhs) noexcept;

        Image& operator=(const Image& rhs);
        Image& operator=(Image&& rhs);


        /* ----- Storage ----- */
        void Convert(
            const ImageFormat& format, 
            const DataType& dataType,
            size_t threadCount = 0
        );
        //void Resize(const Extent& extent, const SamplerFilter& filter);
        void Resize(
            const Extent& extent,
            const ColorRGBA<float>& fillColor, //<! Specifies the color to fill the pixels with that are outside the previous extent.
            const Offset& offset               //<! Specifies the offset to move the previous pixels to. This will be clamped if it exceeds the image area.
        );
        void Swap(Image& rhs);
        void Reset();
        void Reset(
            const Extent& extent, 
            const ImageFormat& format,
            const DataType& dataType, 
            ByteBuffer&& data
        );
        ByteBuffer Release(); //<! Releases the ownership of the image buffer and resets all attributes.


        /* ----- Pixels ----- */
        void Blit(
            const Offset& dstRegionOffset, 
            const Image&  srcImage,
            const Offset& srcRegionOffset, 
            const Extent& srcRegionExtent
        );
        void Fill(
            const Offset& offset,
            const Extent& extent,
            const ColorRGBA<float>& fillColor
        );
        void ReadPixels(
            const Offset& offset,
            const Extent& extent,
            const MutableImageDescriptor& imageDesc,
            size_t threadCount = 0
        ) const;
        void WritePixels(
            const Offset& offset,
            const Extent& extent,
            const ImmutableImageDescriptor& imageDesc,
            size_t threadCount = 0);
        void MirrorYZ();
        void MirrorXZ();
        void MirrorXY();


        /* ----- Attributes ----- */
        ImmutableImageDescriptor GetDescriptor() const;
        MutableImageDescriptor GetMutableDescriptor();
        uint32_t GetBytesPerPixel() const;
        uint32_t GetRowStride() const;
        uint32_t GetDepthStride() const;
        uint32_t GetNumPixels() const;
        uint32_t GetDataSize() const;
        bool IsRegionInside(const Region& region) const;
        const Extent& GetExtent() const { return mExtent; }
        const ImageFormat& GetFormat() const { return mFormat; }
        const DataType& GetDataType() const { return mDataType; }
        const void* GetData() const { return mData.get(); }
        void* GetData() { return mData.get(); }

    protected:
        void ResetAttributes();
        Region ClampRegion(const Region& region) const;
        size_t GetDataAt(const Offset& offset) const;

    protected:
        ImageFormat mFormat = ImageFormat::RGBA;
        DataType mDataType = DataType::UInt8;
        Extent mExtent;
        ByteBuffer mData;
    };
}