#include "Engine/Common/Image.h"
#include "stb/stb_image.h"
#include "stb/stb_image_resize.h"
#include "stb/stb_image_write.h"

namespace ZHU
{
    Image::Image()
    {}

    Image::~Image()
    {}

    bool Image::LoadFile(const char* path)
    {
        unsigned char* data = stbi_load(path, &mWidth, &mHeight, &mChannels, 0);
        if (data == nullptr) {
            Destroy();
            return false;
        }

        size_t size = mWidth * mHeight * mChannels * sizeof(char);
        mData.resize(size);
        std::memcpy(mData.data(), data, size);
        stbi_image_free(data);

        return true;
    }

    bool Image::LoadMemory(const unsigned char* src, size_t size)
    {
        unsigned char* data = stbi_load_from_memory(src, (int)size, &mWidth, &mHeight, &mChannels, 0);
        if (data == nullptr) {
            Destroy();
            return false;
        }

        size_t size_ = mWidth * mHeight * mChannels * sizeof(char);
        mData.resize(size_);
        std::memcpy(mData.data(), data, size_);
        stbi_image_free(data);

        return true;
    }

    bool Image::SaveToFile(const char* path)
    {
        int code = stbi_write_bmp(path, mWidth, mHeight, mChannels, mData.data());
        return code != 0;
    }

    void Image::Destroy()
    {
        mData.clear();
        mWidth = 0;
        mHeight = 0;
        mChannels = 0;
    }

    bool Image::IsValid() const
    {
        return mData.size() > 0 && mWidth > 0 && mHeight > 0 && mChannels > 0;
    }

    int Image::Width() const
    {
        return mWidth;
    }

    int Image::Height() const
    {
        return mHeight;
    }

    int Image::Channels() const
    {
        return mChannels;
    }

    int Image::GetSizeInBytes() const
    {
        return mWidth * mHeight * mChannels * sizeof(char);
    }

    unsigned char* Image::GetData()
    {
        return mData.data();
    }

    const unsigned char* Image::GetData() const
    {
        return mData.data();
    }

    void Image::SetData(const unsigned char* data, size_t size)
    {
        mData.resize(size);
        std::memmove(mData.data(), data, size);
    }

    void Image::SetSize(int width, int height)
    {
        mWidth = width;
        mHeight = height;
    }

    void Image::SetChannels(int channels)
    {
        mChannels = channels;
    }
}