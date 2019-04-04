#pragma once

#include "Common/Config.h"
#include "Scene/SceneNode.h"
#include "Common/Utilities.h"
#include "Scene/MaterialManager.h"
#include "Scene/KDTree.h"

namespace NTGS {
    class Scene;
    typedef std::shared_ptr<Scene> ScenePtr;

    class Scene : public NonCopyable {
    public:
        static ScenePtr Create();

    private:
        KDTree<SceneNodePtr> mSceneTree;
    };
}