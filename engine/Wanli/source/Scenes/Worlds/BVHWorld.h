#pragma once

#include "Scenes/IWorld.h"

namespace Wanli
{
    class DLLDECL BVHWorld final : public IWorld
    {
    public:
        virtual ~BVHWorld();
        virtual bool Initialize() override final;
        virtual void Update() override final;
        virtual void Clear() override final;

        virtual SharedPtr<Entity> CreateEntity(const String & name) override final;
        virtual SharedPtr<Entity> CreateEntity(const Path & path) override final;
        virtual void RemoveEntity(Entity * entity) override final;
        virtual void RemoveEntity(const String & name) override final;
        virtual Entity* GetEntity(const String & name) const override final;

        virtual bool Contains(Entity * entity) const override final;
        virtual Array<Entity*> QueryAll() const override final;
        virtual Array<Entity*> QueryFrustum(const Frustum & frustum) const override final;
        virtual Array<Entity*> QuerySphere(const glm::vec3 & center, float radius) const override final;
        virtual Array<Entity*> QueryBox(const glm::vec3 & min, const glm::vec3 & max) const override final;
    };
}