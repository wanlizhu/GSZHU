#include "GIDeviceContextVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDeviceContextVk> GIDeviceContextVk::Create(SharedPtr<GIDeviceVk> device)
    {
        return SharedPtr<GIDeviceContextVk>(new GIDeviceContextVk(device));
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