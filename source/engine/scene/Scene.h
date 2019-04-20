#pragma once

#include "common/Config.h"
#include "scene/SceneNode.h"
#include "common/Utilities.h"
#include "scene/MaterialManager.h"
#include "scene/KDTree.h"
#include "window/IWindow.h"

namespace NTGS {
    class Scene;
    typedef std::shared_ptr<Scene> ScenePtr;

    class Scene : public NonCopyable
                , std::enable_shared_from_this<Scene> {
    public:
        static ScenePtr Create(const std::string& sceneFile);

        bool LoadScene(const std::string& path);
        bool SaveScene(const std::string& path) const;
        void SetDisplayWindow(const std::shared_ptr<IWindow>& win);

    private:
        KDTree<SceneNode> mSceneTree;
    };
}