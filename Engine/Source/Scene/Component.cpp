#include "Scenes/IComponent.h"

namespace Wanli
{
    bool IComponent::IsAttachedTo(Entity* entity) const
    {
        return std::any_of(mEntities.begin(), mEntities.end(),
                           [&] (std::weak_ptr<Entity> item) {
                               return item.lock().get() == entity;
                           });
    }

    void IComponent::RemoveFromEntity(Entity* entity)
    {
        auto it = std::find_if(mEntities.begin(), mEntities.end(),
                               [&] (std::weak_ptr<Entity> item) {
                                   return item.lock().get() == entity;
                               });
        if (it != mEntities.end())
        {
            mEntities.erase(it);
        }
    }

    bool IComponent::operator==(const IComponent& rhs) const
    {
        return mId == rhs.mId;
    }

    bool IComponent::operator< (const IComponent& rhs) const
    {
        return mId < rhs.mId;
    }
}