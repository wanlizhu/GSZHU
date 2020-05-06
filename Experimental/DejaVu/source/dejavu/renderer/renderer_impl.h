#pragma once

#include "renderer/renderer_interface.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class DJV_API RendererImpl 
        : public RendererInterface
        , public inherit_shared_from_this<RendererInterface, RendererImpl>
    {
    public:
        using inherit_shared_from_this<RendererInterface, RendererImpl>::shared_from_this;
        
    };
}