#pragma once

#include "DeviceObject.h"
#include "ResourceDescriptor.h"

namespace Wanlix
{
    class Resource : public DeviceObject
    {
    public:
        using SharedPtr = std::shared_ptr<Resource>;
        using UniquePtr = std::unique_ptr<Resource>;
        using WeakPtr   = std::weak_ptr<Resource>;
        using Desc      = ResourceDescriptor;

        virtual const Desc& GetDescriptor() const = 0;

    protected:
        Resource(
            const std::string&      name,
            std::shared_ptr<Device> device
        ) noexcept;
    };
}