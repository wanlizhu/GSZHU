#include "World.h"

namespace Wanli
{
    void World::Initialize() 
    {}

    void World::Update()
    {}

    void World::Destroy() 
    {}

    SharedPtr<Entity> World::CreateEntity(const String& name)
    {
        return {};
    }

    SharedPtr<Entity> World::CreateEntity(const Path& path)
    {
        return {};
    }

    void World::RemoveEntity(Entity* entity)
    {}

    void World::RemoveEntity(const String& name)
    {}

    Entity* World::GetEntity(const String& name) const
    {
        return nullptr;
    }

    bool World::Contains(Entity* entity) const
    {
        return false;
    }

    Array<Entity*> World::QueryAll() const
    {
        return {};
    }

    Array<Entity*> World::QueryFrustum(const Frustum& frustum) const
    {
        return {};
    }

    Array<Entity*> World::QuerySphere(const glm::vec3& center, float radius) const
    {
        return {};
    }

    Array<Entity*> World::QueryBox(const glm::vec3& min, const glm::vec3& max) const
    {
        return {};
    }
}