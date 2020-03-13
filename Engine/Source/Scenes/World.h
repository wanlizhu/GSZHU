#pragma once

#include "IComponent.h"
#include "Entity.h"
#include "Utils/Frustum.h"

namespace Wanli
{
    class WANLI_API World : public NonCopyable
        , public std::enable_shared_from_this<World>
    {
    public:
        World() = default;
        virtual ~World() = default;

        void Initialize();
        void Update();
        void Destroy();

        SharedPtr<Entity> CreateEntity(const String& name);
        SharedPtr<Entity> CreateEntity(const Path& path);
        void RemoveEntity(Entity* entity);
        void RemoveEntity(const String& name);
        Entity* GetEntity(const String& name) const;
        
        bool Contains(Entity* entity) const;
        Array<Entity*> QueryAll() const;
        Array<Entity*> QueryFrustum(const Frustum& frustum) const;
        Array<Entity*> QuerySphere(const glm::vec3& center, float radius) const;
        Array<Entity*> QueryBox(const glm::vec3& min, const glm::vec3& max) const;
    
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