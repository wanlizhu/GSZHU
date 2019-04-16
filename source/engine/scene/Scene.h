#pragma once

#include "common/Config.h"
#include "scene/SceneNode.h"
#include "common/Utilities.h"
#include "scene/MaterialManager.h"
#include "scene/KDTree.h"
#include "window/GLWindow.h"

namespace NTGS {
    class Scene;
    typedef std::shared_ptr<Scene> ScenePtr;

    class Scene : public NonCopyable {
    public:
        static ScenePtr Create();

    private:
        KDTree<SceneNodePtr> mSceneTree;
        std::shared_ptr<GLWindow> mpWindow;
    };
}