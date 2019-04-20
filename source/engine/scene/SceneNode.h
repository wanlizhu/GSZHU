#pragma once

#include "common/Config.h"
#include "math/Transform.h"
#include "scene/KDTree.h"
#include "scene/ISceneObject.h"

namespace NTGS {
    class SceneNode;
    typedef std::shared_ptr<SceneNode> SceneNodePtr;

    class SceneNode : public IKDTreePoint<double, 3>
                    , public std::enable_shared_from_this<SceneNode> {
    public:
        static SceneNodePtr Create(ISceneObjectPtr object);

        virtual double operator[](int index) const override;
        bool IsVisible() const;
        const std::string& GetName() const;
        void SetName(const std::string& name);
        Transform& GetWorldTransform();
        const Transform& GetWorldTransform() const;
        Transform& GetParentTransform();
        const Transform& GetParentTransform() const;
        template<typename T> std::shared_ptr<T>& GetObject();
        template<typename T> const std::shared_ptr<T>& GetObject() const;

    private:
        std::string mName;
        Transform mWorldTransform;
        Transform mParentTransform;
        ISceneObjectPtr mpObject;
    };
}