#pragma once

#include "GICommon.h"

namespace GS
{
    class GIDevice : public std::enable_shared_from_this<GIDevice>
    {
    public:
        using Ptr = std::shared_ptr<GIDevice>;
        using ConstPtr = std::shared_ptr<const GIDevice>;
        using Handle = void*;

        static Ptr Create();

        virtual ~GIDevice() { Destroy(); }
        virtual void Init(void* data) = 0;
        virtual void Destroy() = 0;

    private:
        Handle mHandle = nullptr;
    };
}