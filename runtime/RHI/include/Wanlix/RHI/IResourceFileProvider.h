#pragma once

#include "Wanlix/Object.h"
#include "Image.h"

namespace Wanlix
{
    class IResourceFileProvider : public inherit_shared_from_this<Object, IResourceFileProvider>
    {
    public:
        virtual void AddSearchPathes(Array<Wstring> const& pathes) = 0;
        virtual Bool FindFile(Wstring const& path, Wstring* absPath = nullptr) const = 0;
        virtual Size LoadFile(Wstring const& path, ByteArray& data) = 0;
        virtual Size LoadImage(Wstring const& path, Image& image) = 0;
    };
}