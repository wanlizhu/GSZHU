#pragma once

#include "IResourceDesc.h"
#include "CTextureFormat.h"

namespace GSZHU {
    class CTextureDesc : public IResourceDesc {
    public:
        CTextureDesc(CTextureFormat format, EUsage usage, EBindMask bindMask, uint64_t queueMask = 1)
            : IResourceDesc(typeid(CTextureDesc).name())
            , mFormat(format)
            , mUsage(usage)
            , mBindMask(bindMask)
            , mCommandQueueMask(queueMask)
        {}

        CTextureFormat GetFormat() const { return mFormat; }
        EUsage GetUsage() const { return mUsage; }
        EBindMask GetBindMask() const { return mBindMask; }
        uint64_t GetCommandQueueMask() const { return mCommandQueueMask; }

    private:
        CTextureFormat mFormat;
        EUsage mUsage = EUsage::Default;
        EBindMask mBindMask = EBindMask::None;
        uint64_t mCommandQueueMask = 1;
    };


}