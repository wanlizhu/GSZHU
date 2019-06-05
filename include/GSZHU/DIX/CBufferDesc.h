#pragma once

#include "IResourceDesc.h"

namespace GSZHU {
    struct SBufferMode {
        EBuffer Mode = EBuffer::Undefined;
        // For a structured buffer, the size of each buffer element
        // For a formatted buffer, the size of the format that will be used for this buffer's views
        uint32_t ElementSize = 0;

        SBufferMode(EBuffer mode, uint32_t size)
            : Mode(mode)
            , ElementSize(size)
        {}
    };

    class CBufferDesc : public IResourceDesc {
    public:
        CBufferDesc(uint32_t size, EBindMask bindMask, EUsage usage, SBufferMode mode, uint64_t queueMask = 1)
            : IResourceDesc(typeid(CBufferDesc).name())
            , mSizeInBytes(size)
            , mBindMask(bindMask)
            , mUsage(usage)
            , mMode(mode)
            , mCommandQueueMask(queueMask)
        {}

        uint32_t GetSize() const { return mSizeInBytes; }
        uint32_t GetElementSize() const { return mMode.ElementSize; }
        EBindMask GetBindMask() const { return mBindMask; }
        EUsage GetUsage() const { return mUsage; }
        SBufferMode GetMode() const { return mMode; }
        uint64_t GetCommandQueueMask() const { return mCommandQueueMask; }

    private:
        uint32_t mSizeInBytes = 0;
        EBindMask mBindMask = EBindMask::None;
        EUsage mUsage = EUsage::Default;
        SBufferMode mMode;
        uint64_t mCommandQueueMask = 1; // Defines which command queues this buffer can be used with
    };
}