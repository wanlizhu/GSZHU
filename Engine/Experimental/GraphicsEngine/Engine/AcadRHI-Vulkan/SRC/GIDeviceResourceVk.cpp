#include "GIDeviceResourceVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDeviceResourceVk::~GIDeviceResourceVk()
    {}

    GIDeviceResourceVk::GIDeviceResourceVk(SharedPtr<GIDeviceVk> device)
        : GIDeviceObjectVk(device)
    {}
}