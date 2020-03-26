#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;

    class GIDeviceContextVk 
        : public GINonCopyable
        , public std::enable_shared_from_this<GIDeviceContextVk>
    {
    public:
        static SharedPtr<GIDeviceContextVk> Create();

    private:
        GIDeviceContextVk();

    private:
        SharedPtr<GIDeviceVk> mDevice;
    };
}