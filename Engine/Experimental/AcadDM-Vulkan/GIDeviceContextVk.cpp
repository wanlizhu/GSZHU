#include "GIDeviceContextVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDeviceContextVk> GIDeviceContextVk::Create(SharedPtr<GIDeviceVk> device)
    {
        auto raw = new GIDeviceContextVk(device);
        return SharedPtr<GIDeviceContextVk>(raw);
    }

    GIDeviceContextVk::GIDeviceContextVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
    }

    GIDeviceContextVk::~GIDeviceContextVk()
    {}

    void GIDeviceContextVk::Flush()
    {}

}