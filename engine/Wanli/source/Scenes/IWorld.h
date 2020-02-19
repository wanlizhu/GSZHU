#pragma once

#include "Scenes/IComponent.h"
#include "Scenes/Entity.h"
#include "Scenes/Helpers/Frustum.h"

namespace Wanli
{
    class DLLDECL IWorld : public NonCopyable
        , public std::enable_shared_from_this<IWorld>
    {
    public:
        virtual ~IWorld() = default;
        virtual bool Initialize() = 0;
        virtual void Update() = 0;
        virtual void Clear() = 0;

        virtual SharedPtr<Entity> CreateEntity(const String& name) = 0;
        virtual SharedPtr<Entity> CreateEntity(const Path& path) = 0;
        virtual void RemoveEntity(Entity* entity) = 0;
        virtual void RemoveEntity(const String& name) = 0;
        virtual Entity* GetEntity(const String& name) const = 0;
        
        virtual bool Contains(Entity* entity) const = 0;
        virtual Array<Entity*> QueryAll() const = 0;
        virtual Array<Entity*> QueryFrustum(const Frustum& frustum) const = 0;
        virtual Array<Entity*> QuerySphere(const glm::vec3& center, float radius) const = 0;
        virtual Array<Entity*> QueryBox(const glm::vec3& min, const glm::vec3& max) const = 0;
    
        template<typename T>
        inline T* GetEntityOf() const
        {
            auto it = std::find_if(mEntities.begin(), mEntities.end(),
                                   [&] (SharedPtr<Entity> item) {
                                       return dynamic_cast<T*>(item.get()) != nullptr;
                                   });
            return it == mEntities.end() ? nullptr : (*it).get();
        }

        template<typename T>
        inline Array<T*> GetEntitiesOf() const
        {
            Array<T*> entities;
            for (const auto& entity : mEntities)
            {
                if (dynamic_cast<T*>(entity.get()) != nullptr)
                {
                    entities.push_back(entity.get());
                }
            }
            return entities;
        }

    protected:
        Array<UniquePtr<Entity>> mEntities;
    };
}