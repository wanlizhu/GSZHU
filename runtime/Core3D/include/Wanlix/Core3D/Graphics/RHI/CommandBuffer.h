#pragma once

#include "CommandBufferDescriptor.h"
#include "DeviceObject.h"
#include <memory>

namespace Wanlix
{
    class CommandBuffer : public DeviceObject
    {
    public:
        using Ptr      = std::shared_ptr<CommandBuffer>;
        using ConstPtr = std::shared_ptr<const CommandBuffer>;

    };
}