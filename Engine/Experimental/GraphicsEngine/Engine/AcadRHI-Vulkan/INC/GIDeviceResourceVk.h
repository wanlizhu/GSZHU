#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    enum class EResourceType
    {
        Buffer,
        Image,
    };

    class GIDeviceResourceVk : public GIDeviceObjectVk
    {
    public:
        virtual ~GIDeviceResourceVk();
        virtual EResourceType GetResourceType() const = 0;

    protected:
        GIDeviceResourceVk(SharedPtr<GIDeviceVk> device);
    };
}