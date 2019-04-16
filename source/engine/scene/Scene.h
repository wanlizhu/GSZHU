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

    class Scene : public NonCopyable {
    public:
        static ScenePtr Create(const std::string& sceneFile);

        Scene();
        virtual ~Scene();

        void SetDisplayWindow(const std::shared_ptr<IWindow>& win);

    private:
        bool LoadSceneFile(const std::string& sceneFile);
        bool SaveSceneFile();
        void ConnectWindowEvents();

    private:
        KDTree<SceneNodePtr> mSceneTree;
        std::shared_ptr<IWindow> mpWindow;
        std::string mSceneFile;
    };
}