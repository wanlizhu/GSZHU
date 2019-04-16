#pragma once

#include "common/Config.h"
#include "math/Math.h"

namespace NTGS {
    class ISceneObject;
    typedef std::shared_ptr<ISceneObject> ISceneObjectPtr;

    class ISceneObject {
    public:
        virtual Vec3d GetCenter() const = 0;
    };
}