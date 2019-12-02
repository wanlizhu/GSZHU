#pragma once

#include "Wanlix/Core3D/Graphics/GObject.h"

namespace Wanlix
{
    class PipelineLayout : public GObject
    {
    public:
        using Ptr = std::shared_ptr<PipelineLayout>;
        using ConstPtr = std::shared_ptr<const PipelineLayout>;
    };
}