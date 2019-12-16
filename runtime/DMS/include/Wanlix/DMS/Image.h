#pragma once

#include "Wanlix/Object.h"
#include "Common.h"

namespace Wanlix
{
    struct Image
    {
        Uint width = 0;
        Uint height = 0;
        DataType componentType = DataType::Undefined;
        Uint components = 0;
        Uint rowStride = 0;
        SharedPtr<DataBlock> dataBlock;
        String filePath;
    };

    Bool LoadImageFile(Wstring const& path, Image& image);
    Bool SaveImageFile(Wstring const& path, Image const& image);
}