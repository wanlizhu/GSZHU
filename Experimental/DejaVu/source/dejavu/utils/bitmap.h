#pragma once

#include <string>
#include <memory>
#include <optional>
#include "utils/preprocess.h"
#include "utils/filesystem.h"

namespace djv
{
    /*
     * Supports: Auto, BMP, HDR, JPG, PNG, TGA.
    */
    class DJV_API Bitmap
    {
    public:
        Bitmap(const std::string& path, int channels, bool floatdata);
        Bitmap(int width, int height, int channels, bool floatdata);

        void setPath(const std::string& path);
        void setData(int offset, int size, const void* data);
        void flush();
        bool load(const std::string& path, int channels, bool floatdata);

        std::string path() const;
        const void* data() const;
        void* data();
        bool isFloat() const;
        int width() const;
        int height() const;
        int channels() const;
        
    private:
        int mWidth = 0;
        int mHeight = 0;
        int mChannels = 0;
        bool mIsFloatData = false;
        std::optional<std::string> mFilePath;
        std::shared_ptr<uint8_t> mBytes;
    };
}