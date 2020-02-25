#include "Utils/Bitmap.h"
#include "Utils/ElapsedTime.h"
#include "Utils/File.h"
#include "Utils/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Wanli
{
    Bitmap::Bitmap(const Path& path)
        : mFilePath(path)
    {
        Load(mFilePath);
    }

    Bitmap::Bitmap(const glm::ivec2& size, int bytesPerPixel)
        : mData(std::make_unique<Byte[]>(size.x * size.y * bytesPerPixel))
        , mSize(size)
        , mBytesPerPixel(bytesPerPixel)
    {}

    Bitmap::Bitmap(const glm::ivec2& size, int bytesPerPixel, UniquePtr<Byte[]>&& data)
        : mData(std::move(data))
        , mSize(size)
        , mBytesPerPixel(bytesPerPixel)
    {}

    void Bitmap::Load(const Path& path)
    {
#ifdef WANLI_DEBUG
        DeltaTime debugTimer;
#endif
        auto encoder = Registry().find(Path(path).extension().string());
        if (encoder != Registry().end())
        {
            encoder->second.loadMethod(this, path.string().c_str());
        }
        else // Fallback to stb_image
        {
            auto data = File(path).ReadFile();
            if (!data)
            {
                LOG_ERROR("!!! Failed to load image file: \"%s\"!!!\n", path.c_str());
                return;
            }

            mData = UniquePtr<Byte[]>(stbi_load_from_memory(
                reinterpret_cast<Byte*>(data->data()),
                static_cast<uint32_t>(data->size()),
                reinterpret_cast<int*>(&mSize.x),
                reinterpret_cast<int*>(&mSize.y),
                reinterpret_cast<int*>(&mBytesPerPixel),
                STBI_rgb_alpha));
            mBytesPerPixel = 4;
        }

#ifdef WANLI_DEBUG
        LOG_DEBUG("Bitmap \"%s\" loaded in %.0f Ms.\n", 
                  Path(path).filename().c_str(),
                  debugTimer.Get<MilliSecond>());
#endif
    }

    void Bitmap::Write(const Path& path) const
    {
#ifdef WANLI_DEBUG
        DeltaTime debugTimer;
#endif
        if (auto parentPath = Path(path).parent_path();
            !parentPath.empty())
        {
            std::filesystem::create_directories(parentPath);
        }

        if (auto it = Registry().find(Path(path).extension().string());
            it != Registry().end())
        {
            it->second.writeMethod(this, path.string().c_str());
        }
        else // Fallback to stb_image
        {
            int len = 0;
            UniquePtr<Byte[]> pngData(stbi_write_png_to_mem(mData.get(),
                                      mSize.x * mBytesPerPixel,
                                      mSize.x,
                                      mSize.y,
                                      mBytesPerPixel,
                                      &len));
            File(path).WriteFile(reinterpret_cast<char*>(pngData.get()), len);
        }

#ifdef WANLI_DEBUG
        LOG_DEBUG("Bitmap \"%s\" written in %.0f Ms.\n",
                  Path(path).filename().c_str(),
                  debugTimer.Get<MilliSecond>());
#endif
    }
}