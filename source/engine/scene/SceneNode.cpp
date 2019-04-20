#include "SceneNode.h"

namespace NTGS {
    static SceneNodePtr Create(ISceneObjectPtr object);

    virtual double operator[](int index) const override;
    bool IsVisible() const;
    const std::string& GetName() const { return mName; }
    void SetName(const std::string& name) { mName = name; }
    Transform& GetWorldTransform() { return mWorldTransform; }
    const Transform& GetWorldTransform() const { return mWorldTransform; }
    template<typename T> std::shared_ptr<T>& GetObject() { return std::dynamic_pointer_cast<T>(mObject); }

}