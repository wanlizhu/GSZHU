#pragma once

#include "DeviceObject.h"
#include "EResourceState.h"

namespace GS2::GI
{
    class Resource : public DeviceObject
    {
    public:
        enum class EType
        {
            Undefined = 0,
            Buffer,
            Texture1D,
            Texture2D,
            Texture3D,
            TextureCube,
            Texture2DMS, // 2D multi-sampled texture
        };

        virtual void init() override;
        virtual void destroy() override;
        virtual operator bool() const override;
        
    protected:
        Resource(const char* name, Device::SharedPtr device, EType type);

    protected:
        EType _type = EType::Undefined;
        EResourceState _state = EResourceState::Undefined;
    };
}