#pragma once

#include "Engine/Common/Config.h"
#include <vector>

namespace ZHU
{
    class Image
    {
    public:
        Image();
        virtual ~Image();

        bool LoadFile(const char* path);
        bool LoadMemory(const unsigned char* src, size_t size);
        bool SaveToFile(const char* path);
        void Destroy();
        
        bool IsValid() const;
        int Width() const;
        int Height() const;
        int Channels() const;
        int GetSizeInBytes() const;
        unsigned char* GetData();
        const unsigned char* GetData() const;

        void SetData(const unsigned char* data, size_t size);
        void SetSize(int width, int height);
        void SetChannels(int channels);

    protected:
        std::vector<unsigned char> mData;
        int mWidth = 0;
        int mHeight = 0;
        int mChannels = 0;
    };
}