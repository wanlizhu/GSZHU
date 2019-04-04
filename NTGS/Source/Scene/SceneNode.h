#pragma once

#include "Common/Config.h"
#include "Math/Transform.h"
#include "Scene/KDTree.h"
#include "Scene/ISceneObject.h"

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
        template<typename T>
        T& GetObject() { return std::dynamic_pointer_cast<T>(mObject); }

    private:
        std::string mName;
        Transform mTransform;
        ISceneObjectPtr mObject;
    };
}