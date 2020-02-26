#include "Entity.h"

namespace Wanli
{
    bool Entity::Initialize()
    {
        return true;
    }

    void Entity::Update()
    {
    }

    bool Entity::LoadPrefab(const Path& path)
    {
        return true;
    }

    Entity& Entity::AddComponent(IComponent* comp)
    {
        mComponents.push_back(comp->shared_from_this());
        return *this;
    }

    Entity& Entity::RemoveComponent(IComponent* comp)
    {
        auto it = std::find_if(mComponents.begin(), mComponents.end(),
                               [&] (SharedPtr<IComponent> item) {
                                   return item.get() == comp;
                               });
        if (it != mComponents.end())
        {
            mComponents.erase(it);
        }
        return *this;
    }

    Entity& Entity::RemoveComponent(const String& name)
    {
        auto it = std::find_if(mComponents.begin(), mComponents.end(),
                               [&] (SharedPtr<IComponent> item) {
                                   return item->GetName() == name;
                               });
        if (it != mComponents.end())
        {
            (*it)->RemoveFromEntity(this);
            mComponents.erase(it);
        }
        return *this;
    }

    IComponent* Entity::GetComponent(const String& name) const
    {
        auto it = std::find_if(mComponents.begin(), mComponents.end(),
                               [&] (SharedPtr<IComponent> item) {
                                   return item->GetName() == name;
                               });
        if (it != mComponents.end())
        {
            return (*it).get();
        }
        return nullptr;
    }
}