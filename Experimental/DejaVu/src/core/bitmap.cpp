#include "dejavu/core/bitmap.h"
#include "dejavu/core/strings.h"
#include <assert.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

namespace djv
{
    Bitmap::Bitmap(const std::string& path, int channels, bool floatdata)
        : mFilePath(path)
        , mChannels(channels)
        , mIsFloatData(floatdata)
    {
        load(path, channels, floatdata);
    }

    Bitmap::Bitmap(int width, int height, int channels, bool floatdata)
        : mFilePath(std::nullopt)
        , mWidth(width)
        , mHeight(height)
        , mChannels(channels)
        , mIsFloatData(floatdata)
        , mBytes(nullptr)
    {
        int bytes_count = width * height * channels * (floatdata ? sizeof(float) : sizeof(char));
        mBytes = std::shared_ptr<uint8_t>(new uint8_t[bytes_count], 
                                          [&](const uint8_t* data) {
                                              if (data) {
                                                  delete[] data;
                                              }
                                          });
    }

    void Bitmap::setPath(const std::string& path)
    {
        mFilePath = path;
    }

    void Bitmap::setData(int offset, int size, const void* data)
    {
        int bytes_count = mWidth * mHeight * mChannels * (mIsFloatData ? sizeof(float) : sizeof(char));
        assert(offset + size <= bytes_count);

        std::memmove(mBytes.get() + offset, data, size);
    }

    void Bitmap::flush()
    {
        if (istrcmp(mFilePath.value(), ".bmp") == 0)
        {
            stbi_write_bmp(mFilePath.value().c_str(), mWidth, mHeight, mChannels, mBytes.get());
        }
        else if (istrcmp(mFilePath.value(), ".hdr") == 0)
        {
            assert(mIsFloatData);
            stbi_write_hdr(mFilePath.value().c_str(), mWidth, mHeight, mChannels, (const float*)mBytes.get());
        }
        else if (istrcmp(mFilePath.value(), ".jpg") == 0)
        {
            stbi_write_jpg(mFilePath.value().c_str(), mWidth, mHeight, mChannels, mBytes.get(), 0);
        }
        else if (istrcmp(mFilePath.value(), ".png") == 0)
        {
            stbi_write_png(mFilePath.value().c_str(), mWidth, mHeight, mChannels, mBytes.get(), mWidth);
        }
        else if (istrcmp(mFilePath.value(), ".tga") == 0)
        {
            stbi_write_tga(mFilePath.value().c_str(), mWidth, mHeight, mChannels, mBytes.get());
        }
    }

    bool Bitmap::load(const std::string& path, int channels, bool floatdata)
    {
        FILE* file = fopen(path.c_str(), "rb");
        if (file == nullptr)
            return false;

        uint8_t* bytes = nullptr;
        mChannels = channels;
        mIsFloatData = floatdata;

        if (mIsFloatData)
        {
            bytes = (uint8_t*)stbi_loadf_from_file(file, &mWidth, &mHeight, &mChannels, mChannels);
        }
        else
        {
            bytes = (uint8_t*)stbi_load_from_file(file, &mWidth, &mHeight, &mChannels, mChannels);
        }

        mBytes = std::shared_ptr<uint8_t>(bytes,
                                          [&](const uint8_t* data) {
                                              if (data) {
                                                  stbi_image_free((void*)data);
                                              }
                                          });
        fclose(file);
        file = nullptr;

        return mBytes != nullptr;
    }

    std::string Bitmap::path() const
    {
        return mFilePath.has_value() ? mFilePath.value() : "";
    }

    const void* Bitmap::data() const
    {
        return mBytes.get();
    }

    void* Bitmap::data()
    {
        return mBytes.get();
    }

    int Bitmap::width() const
    {
        return mWidth;
    }

    int Bitmap::height() const
    {
        return mHeight;
    }
    
    int Bitmap::channels() const
    {
        return mChannels;
    }

    bool Bitmap::isFloat() const
    {
        return mIsFloatData;
    }
}