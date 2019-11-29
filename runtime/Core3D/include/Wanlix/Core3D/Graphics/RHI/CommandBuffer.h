#pragma once

#include "Wanlix/Core3D/Graphics/GObject.h"
#include <memory>

namespace Wanlix
{
    class CommandBuffer : public GObject
    {
    public:
        using Ptr      = std::shared_ptr<CommandBuffer>;
        using ConstPtr = std::shared_ptr<const CommandBuffer>;

    };
}