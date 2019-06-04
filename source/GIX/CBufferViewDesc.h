#pragma once

#include "IResourceDesc.h"
#include "CBufferFormat.h"

namespace GSZHU {
    class CBufferViewDesc : public IResourceDesc {
    public:
        CBufferViewDesc(EBufferViewType type, CBufferFormat format, uint32_t offset, uint32_t size)
            : IResourceDesc(typeid(CBufferFormat).name())
            , mType(type)
            , mFormat(format)
            , mOffset(offset)
            , mSize(size)
        {}

        EBufferViewType GetViewType() const { return mType; }
        CBufferFormat GetFormat() const { return mFormat; }
        int32_t GetOffset() const { return mOffset; }
        int32_t GetSize() const { return mSize; }

    private:
        EBufferViewType mType = EBufferViewType::Undefined;
        CBufferFormat mFormat;
        uint32_t mOffset = 0;
        uint32_t mSize = 0;
    };
}