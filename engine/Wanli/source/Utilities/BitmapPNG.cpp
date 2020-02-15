#include "Utilities/BitmapPNG.h"
#include "HAL/File.h"
#include "Utilities/ElapsedTime.h"
#include "Utilities/Log.h"
#include "LodePNG/lodepng.h"

namespace Wanli
{
    bool BitmapPNG::smRegistered = BitmapPNG::Register(".png");

    void BitmapPNG::Load(Bitmap* bitmap, const String& path)
    {
#ifdef WANLI_DEBUG
        DeltaTime debugTimer;
#endif
        auto data = File(path).ReadFile();
        if (!data)
        {
            LOG_ERROR("!!! Failed to read file: \"%s\"\n", path.c_str());
            return;
        }

        Byte* buffer = nullptr;
        Uint width = 0;
        Uint height = 0;
        auto error = lodepng_decode_memory(&buffer,
                                           &width,
                                           &height,
                                           reinterpret_cast<const Byte*>(data->data()),
                                           data->size(),
                                           LCT_RGBA, 
                                           8);
        if (buffer && !error)
        {
            LodePNGColorMode colorMode = {};
            colorMode.colortype = LCT_RGBA;
            colorMode.bitdepth = 8;

            auto buffersize = (int)lodepng_get_raw_size(width, height, &colorMode);
            bitmap->SetData(std::make_unique<Byte[]>(buffersize));
            std::memcpy(bitmap->GetData().get(), buffer, buffersize);
            
            bitmap->SetSize({ width, height });
            bitmap->SetBytesPerPixel(buffersize / (width * height));

            //lodepng_free(buffer);
            free(buffer);
        }

#ifdef WANLI_DEBUG
        LOG_DEBUG("BitmapPNG \"%s\" loaded in %.0f Ms.\n",
                  Path(path).filename().c_str(),
                  debugTimer.Get<MilliSecond>());
#endif
    }

    void BitmapPNG::Write(const Bitmap* bitmap, const String& path)
    {
#ifdef WANLI_DEBUG
        DeltaTime debugTimer;
#endif

        LodePNGColorType colorType = LCT_GREY;
        if (bitmap->GetBytesPerPixel() == 4)
        {
            colorType = LCT_RGBA;
        }
        else if (bitmap->GetBytesPerPixel() == 3)
        {
            colorType = LCT_RGB;
        }
        else
        {
            LOG_ERROR("!!! Cannot write PNG with %d bytes per pixel !!!\n", bitmap->GetBytesPerPixel());
        }

        lodepng::encode(path,
                        bitmap->GetData().get(),
                        bitmap->GetSize().x,
                        bitmap->GetSize().y,
                        colorType);

#ifdef WANLI_DEBUG
        LOG_DEBUG("BitmapPNG \"%s\" written in %.0f Ms.\n",
                  Path(path).filename().c_str(),
                  debugTimer.Get<MilliSecond>());
#endif
    }
}