#pragma once

#include <cstdint>
#include "ResourceDescriptor.h"
#include "DeviceObject.h"

namespace Wanlix
{
    /*
    * Base class for all hardware resource interfaces.
    * Buffer
    * Texture
    */
    class Resource : public DeviceObject
    {
    public:
        using Ptr      = std::shared_ptr<Resource>;
        using ConstPtr = std::shared_ptr<const Resource>;

        virtual const ResourceType& GetResourceType() const = 0;
        virtual const ResourceDescriptor& GetResourceDescriptor() const = 0;

    protected:
        Resource(const std::string& name) noexcept;
    };
}