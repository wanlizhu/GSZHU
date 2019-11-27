#pragma once

#include <cstdint>
#include "IDeviceObject.h"
#include "ResourceDescriptor.h"

namespace Wanlix
{
    /*
    * Base class for all hardware resource interfaces.
    * Buffer
    * Texture
    * Sampler
    */
    class IResource : public IDeviceObject
    {
    public:
        using Ptr = std::shared_ptr<IResource>;
        using ConstPtr = std::shared_ptr<const IResource>;

        virtual ResourceType GetResourceType() const = 0;
        virtual ResourceDescriptor GetResourceDescriptor() const = 0;

    protected:
        IResource(const std::string& name) noexcept;
    };
}