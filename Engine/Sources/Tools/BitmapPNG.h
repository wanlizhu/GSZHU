#pragma once

#include "Utilities/Bitmap.h"

namespace Wanli
{
    class BitmapPNG : public Bitmap::Registrar<BitmapPNG>
    {
    public:
        static void Load(Bitmap* bitmap, const String& path);
        static void Write(const Bitmap* bitmap, const String& path);

    private:
        static bool smRegistered;
    };
}