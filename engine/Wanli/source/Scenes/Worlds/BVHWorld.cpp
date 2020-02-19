#include "Scenes/Worlds/BVHWorld.h"

namespace Wanli
{
    BVHWorld::~BVHWorld()
    {}

    bool BVHWorld::Initialize()
    {
        return true;
    }

    void BVHWorld::Update()
    {

    }

    void BVHWorld::Clear()
    {
    
    }

    SharedPtr<Entity> BVHWorld::CreateEntity(const String& name)
    {
        return nullptr;
    }

    SharedPtr<Entity> BVHWorld::CreateEntity(const Path& path)
    {
        return nullptr;
    }

    void BVHWorld::RemoveEntity(Entity * entity)
    {

    }

    void BVHWorld::RemoveEntity(const String& name)
    {

    }

    Entity* BVHWorld::GetEntity(const String& name) const
    {
        return nullptr;
    }

    bool BVHWorld::Contains(Entity * entity) const 
    {
        return false;
    }

    Array<Entity*> BVHWorld::QueryAll() const
    {
        Array<Entity*> result;

        return result;
    }

    Array<Entity*> BVHWorld::QueryFrustum(const Frustum& frustum) const
    {
        Array<Entity*> result;

        return result;
    }

    Array<Entity*> BVHWorld::QuerySphere(const glm::vec3& center, float radius) const
    {
        Array<Entity*> result;

        return result;
    }

    Array<Entity*> BVHWorld::QueryBox(const glm::vec3& min, const glm::vec3& max) const
    {
        Array<Entity*> result;

        return result;
    }

}