#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"
#include "Wanlix/Core3D/Graphics/Descriptors.h"

namespace Wanlix
{
    class Image
    {
    public:
        using Ptr       = std::shared_ptr<Image>;
        using ConstPtr  = std::shared_ptr<const Image>;
        using Desc      = ImageDescriptor;
        using ConstDesc = ConstImageDescriptor;

        Image() noexcept = default;
        explicit Image(
            const Extent&      extent,
            const ImageFormat& format = ImageFormat::RGBA,
            const DataType&    dataType = DataType::UInt8,
            ByteBuffer&&       data = nullptr,
            const Color4F&     fillColor = Color4F::Null()
        ) noexcept;
        Image(const Image& rhs) noexcept;
        Image(Image&& rhs) noexcept;

        Image& operator=(const Image& rhs);
        Image& operator=(Image&& rhs);


        /* ----- Storage ----- */
        void Convert(
            const ImageFormat& format, 
            const DataType&    dataType,
            size_t             threadCount = 0
        );
        //void Resize(const Extent& extent, const SamplerFilter& filter);
        void Resize(
            const Extent&  extent,
            const Color4F& fillColor, //<! Specifies the color to fill the pixels with that are outside the previous extent.
            const Offset&  offset     //<! Specifies the offset to move the previous pixels to. This will be clamped if it exceeds the image area.
        );
        void Swap(Image& rhs);
        void Reset();
        void Reset(
            const Extent&      extent, 
            const ImageFormat& format,
            const DataType&    dataType, 
            ByteBuffer&&       data
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
            const Offset&  offset,
            const Extent&  extent,
            const Color4F& fillColor
        );
        void ReadPixels(
            const Offset& offset,
            const Extent& extent,
            const Desc&   imageDesc,
            size_t threadCount = 0
        ) const;
        void WritePixels(
            const Offset&    offset,
            const Extent&    extent,
            const ConstDesc& imageDesc,
            size_t threadCount = 0);
        void MirrorYZ();
        void MirrorXZ();
        void MirrorXY();


        /* ----- Attributes ----- */
        ConstDesc GetDescriptor() const;
        Desc      GetDescriptor();
        uint32_t  GetBytesPerPixel() const;
        uint32_t  GetRowStride() const;
        uint32_t  GetDepthStride() const;
        uint32_t  GetNumPixels() const;
        uint32_t  GetDataSize() const;
        bool      IsRegionInside(const Region& region) const;
        const Extent&      GetExtent() const    { return mExtent; }
        const ImageFormat& GetFormat() const    { return mFormat; }
        const DataType&    GetDataType() const  { return mDataType; }
        const void* GetData() const             { return mData.get(); }
        void*       GetData()                   { return mData.get(); }

    protected:
        void   ResetAttributes();
        Region ClampRegion(const Region& region) const;
        size_t GetDataAt(const Offset& offset) const;

    protected:
        ImageFormat mFormat   = ImageFormat::RGBA;
        DataType    mDataType = DataType::UInt8;
        Extent      mExtent;
        ByteBuffer  mData;
    };


    /* ----- Functions ----- */


    SubresourceLayout ComputeSubresourceLayout(const Format& format, const Extent& extent);
    Offset ComputeTextureOffset(const TextureType& type, const Offset& offset, uint32_t baseArrayLayer);
    Extent ComputeTextureExtent(const TextureType& type, const Extent& extent, uint32_t numArrayLayers);
    bool     MustGenerateMipsOnCreate(const TextureDescriptor& textureDesc);
    uint32_t GetClampedSamples(uint32_t samples);
    uint32_t ImageFormatSize(const ImageFormat& imageFormat);
    uint32_t ImageDataSize(const ImageFormat& imageFormat, const DataType& dataType, uint32_t numPixels);
    bool     IsCompressedFormat(const ImageFormat& imageFormat);
    bool     IsDepthStencilFormat(const ImageFormat& imageFormat);

    bool ConvertImageBuffer(
        const ConstImageDescriptor& srcImageDesc,
        const ImageDescriptor&      dstImageDesc,
        size_t threadCount = 0
    );
    ByteBuffer ConvertImageBuffer(
        const ConstImageDescriptor& srcImageDesc,
        ImageFormat  dstFormat,
        DataType     dstDataType,
        std::size_t  threadCount = 0
    );
    ByteBuffer GenerateByteBuffer(
        size_t bufferSize,
        bool   initialize = true
    );
    ByteBuffer GenerateImageBuffer(
        ImageFormat    format,
        DataType       dataType,
        size_t         imageSize,
        const Color4F& fillColor
    );
    void CopyImageBufferRegion(
        // Destination
        const ImageDescriptor& dstImageDesc,
        const Offset&          dstOffset,
        uint32_t               dstRowStride,
        uint32_t               dstSliceStride,

        // Source
        const ConstImageDescriptor& srcImageDesc,
        const Offset&               srcOffset,
        uint32_t                    srcRowStride,
        uint32_t                    srcSliceStride,

        // Region
        const Extent& extent
    );
}