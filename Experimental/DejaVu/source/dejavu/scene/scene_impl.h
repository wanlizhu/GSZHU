#pragma once

#include "scene/scene_interface.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class DJV_API SceneImpl 
        : public SceneInterface
        , public inherit_shared_from_this<SceneInterface, SceneImpl>
    {
    public:
        using inherit_shared_from_this<SceneInterface, SceneImpl>::shared_from_this;
        
    };
}