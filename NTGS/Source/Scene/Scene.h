#pragma once

#include "Common/Config.h"
#include "Scene/ISceneNode.h"
#include "Common/Utilities.h"
#include "Scene/MaterialManager.h"

namespace NTGS {
    class Scene;
    typedef std::shared_ptr<Scene> ScenePtr;

    class Scene : public NonCopyable {
    public:
        static ScenePtr Create();

    private:
        SceneNode<void> mRootNode;
    };
}