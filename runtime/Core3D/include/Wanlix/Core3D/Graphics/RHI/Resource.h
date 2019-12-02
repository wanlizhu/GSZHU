#pragma once

#include "Wanlix/Core3D/Graphics/GObject.h"
#include "Wanlix/Core3D/Graphics/Descriptors.h"

namespace Wanlix
{
    class Resource : public GObject
    {
    public:
        using Ptr      = std::shared_ptr<Resource>;
        using ConstPtr = std::shared_ptr<const Resource>;
        using Desc     = ResourceDescriptor;

        virtual const ResourceType& GetResourceType() const = 0;
        virtual const Desc&         GetDescriptor() const = 0;

    protected:
        Resource(
            StringCRef name,
            std::shared_ptr<Device> device
        ) noexcept;
    };
}