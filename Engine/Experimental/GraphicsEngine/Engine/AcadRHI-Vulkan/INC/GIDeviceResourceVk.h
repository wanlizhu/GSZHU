#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceResourceVk : public GIDeviceObjectVk
    {
    public:
        enum class EResourceType
        {
            Buffer,
            Image,
        };

        virtual ~GIDeviceResourceVk() = default;
        virtual EResourceType GetResourceType() const = 0;

    protected:
        GIDeviceResourceVk(SharedPtr<GIDeviceVk> device);
    };
}