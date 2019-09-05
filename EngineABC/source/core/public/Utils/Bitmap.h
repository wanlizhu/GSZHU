#pragma once

#include "Framework.h"
#include <vector>
#include "RHI/Formats.h"
#include "Utils/Object.h"

namespace GS
{
	class ITexture;
	//todo
    class GS_API Bitmap : public Object,
		                  public inherit_shared_from_this<Object, Bitmap>
    {
		GS_OBJECT
    public:
		enum class EExportFlags : uint32_t
		{
			None = 0,
			ExportAlpha = 1,
			Lossy = 2,
			Uncompressed = 4,
		};

		enum class EFileFormat
		{
			PNG,  // lossless compressed 8-bits images with optional alpha
			JPEG, // lossy compressed 8-bits images without alpha
			TGA,  // lossless uncompressed 8-bits images with optional alpha
			BMP,  // lossless uncompressed 8-bits images with optional alpha
			PFM,  // floating point HDR images with 32-bit float per channel
			EXR,  // floating point HDR images with 16-bit float per channel
		};

		using UniquePtr = std::unique_ptr<Bitmap>;
		using UniqueConstPtr = std::unique_ptr<const Bitmap>;

		Bitmap();
        virtual ~Bitmap();

		// If isFlipOver is true, the top-left pixel is the first pixel in the buffer
		static UniqueConstPtr Create(const std::wstring& filename);
		static UniqueConstPtr Create(uint32_t width, 
									 uint32_t height,
									 EResourceFormat format,
									 const void* data = nullptr);
		static void Save(const std::wstring& filename, 
						 EFileFormat fileFormat,
						 EExportFlags flags,
						 UniqueConstPtr bitmap);
		static void Save(const std::wstring& filename, 
						 uint32_t width, 
						 uint32_t height,
						 EFileFormat fileFormat,
						 EExportFlags exportFlags,
						 EResourceFormat resourceFormat,
						 const void* data);
		static void SaveDialog(std::shared_ptr<const ITexture> texture);

		Bitmap() = default;
		virtual ~Bitmap();

		inline const uint8_t* GetData() const { return mData.data(); }
		inline uint8_t* GetData() { return mData.data(); }
		inline uint32_t GetWidth() const { return mWidth; }
		inline uint32_t GetHeight() const { return mHeight; }
		EResourceFormat GetFormat() const { return mFormat; }

    protected:
        std::vector<unsigned char> mData;
        int mWidth = 0;
        int mHeight = 0;
		EResourceFormat mFormat;
    };
}