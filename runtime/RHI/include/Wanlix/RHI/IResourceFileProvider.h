#pragma once

#include "Wanlix/RHI/Common.h"

namespace Wanlix
{
    class RHI_API IResourceFileProvider : public Object
    {
        CLASSINFO(IResourceFileProvider)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual void AddSearchPathes(Array<Wstring> const& pathes) = 0;
        virtual Bool FindFile(Wstring const& path, Wstring* absPath = nullptr) const = 0;
        virtual Size LoadFile(Wstring const& path, ByteArray& data) = 0;
        virtual Size LoadImage(Wstring const& path, Image& image) = 0;
    };
}