#pragma once

#include "Common/Config.h"
#include "Math/Transform.h"
#include "Scene/TreeNode.h"

namespace NTGS {
    class ISceneNode;
    typedef std::shared_ptr<ISceneNode> ISceneNodePtr;

    class ISceneNode : public TreeNode {
    public:
        bool IsVisible() const;

        const std::string& GetName() const;
        void SetName(const std::string& name);
        Transform& GetTransform();
        const Transform& GetTransform() const;

    protected:
        std::string mName;
        Transform mTransform;
    };


    template<typename OBJECT>
    class SceneNode : public ISceneNode {
    public:
        SceneNode() = default;
        SceneNode(const OBJECT::Pointer& obj);
        virtual ~SceneNode() = default;

    private:
        typename OBJECT::Pointer mObject;
    };
}