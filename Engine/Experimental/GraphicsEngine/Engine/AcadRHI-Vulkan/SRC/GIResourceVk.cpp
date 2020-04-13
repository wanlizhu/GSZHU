#include "GIResourceVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIResourceVk::~GIResourceVk()
    {}

    GIResourceVk::GIResourceVk(SharedPtr<GIDeviceVk> device)
        : GIDeviceObjectVk(device)
    {}
}