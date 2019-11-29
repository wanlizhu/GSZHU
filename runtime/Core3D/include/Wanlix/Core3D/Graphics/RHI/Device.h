#pragma once

#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include "Wanlix/Core3D/Types.h"

namespace Wanlix
{
    class Device : public std::enable_shared_from_this<Device>
    {
    public:
        using Handle   = void*;
        using Ptr      = std::shared_ptr<Device>;
        using ConstPtr = std::shared_ptr<const Device>;
        using WeakPtr  = std::weak_ptr<Device>;

        
    };
}