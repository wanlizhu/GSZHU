#pragma once

#include "GIDeviceObjectVk.h"
#include "GIResourceStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    enum class EResourceType
    {
        Buffer,
        Image,
    };

    class GIResourceVk : public GIDeviceObjectVk
    {
    public:
        virtual ~GIResourceVk();
        virtual EResourceType GetResourceType() const = 0;
        virtual GIResourceStateVk& GetResourceState() = 0;
        virtual void TransitionState(const GIResourceStateVk& newState) = 0;
        
    protected:
        GIResourceVk(SharedPtr<GIDeviceVk> device);
    };
}