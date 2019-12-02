#pragma once

#include "Wanlix/Core3D/Graphics/GObject.h"
#include "Wanlix/Core3D/Graphics/Types.h"
#include "Wanlix/Core3D/Graphics/Flags.h"
#include "Buffer.h"
#include "Texture.h"

namespace Wanlix
{
    class SwapChain;

    class CommandBuffer : public GObject
    {
    public:
        using Ptr      = std::shared_ptr<CommandBuffer>;
        using ConstPtr = std::shared_ptr<const CommandBuffer>;

        
    };
}