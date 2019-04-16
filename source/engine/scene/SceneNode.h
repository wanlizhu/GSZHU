#pragma once

#include "common/Config.h"
#include "math/Transform.h"
#include "scene/KDTree.h"
#include "scene/ISceneObject.h"

namespace NTGS {
    class SceneNode;
    typedef std::shared_ptr<SceneNode> SceneNodePtr;

    class SceneNode {
    public:
        SceneNode() = default;
        SceneNode(const ISceneObjectPtr& obj);
        virtual ~SceneNode() = default;

        bool IsVisible() const;

        const std::string& GetName() const { return mName; }
        void SetName(const std::string& name) { mName = name; }
        Transform& GetTransform() { return mTransform; }
        const Transform& GetTransform() const { return mTransform; }
        template<typename T> std::shared_ptr<T>& GetObject() { return std::dynamic_pointer_cast<T>(mObject); }

    private:
        std::string mName;
        Transform mTransform;
        ISceneObjectPtr mpObject;
    };
}