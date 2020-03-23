#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;

    class GIDeviceContextVk : public std::enable_shared_from_this<GIDeviceContextVk>
    {
    public:
        static SharedPtr<GIDeviceContextVk> Create();

    private:
        GIDeviceContextVk();
        GIDeviceContextVk(const GIDeviceContextVk&) = delete;
        GIDeviceContextVk(GIDeviceContextVk&&) = default;
        GIDeviceContextVk& operator=(const GIDeviceContextVk&) = delete;
        GIDeviceContextVk& operator=(GIDeviceContextVk&&) = default;

    private:
        SharedPtr<GIDeviceVk> mDevice;
    };
}