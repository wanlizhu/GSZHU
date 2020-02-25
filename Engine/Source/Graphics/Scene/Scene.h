#pragma once

#include "Core/Common.h"
#include "Core/IModule.h"
#include "Utils/Delegate.h"
#include "Utils/NonCopyable.h"
#include "Utils/Frustum.h"
#include "Components/Camera.h"
#include "World.h"

namespace Wanli
{
    class WANLI_API Scene : public IModule::Registrar<Scene, EModuleStage::Update>
        , virtual public Observer
    {
    public:
        explicit Scene(const String& name = "Unnamed")
            : mName(name)
        {}

        virtual ~Scene() = default;
        virtual void Initialize() override final;
        virtual void Update() override final;
        virtual void Destroy() override final;

        bool Load(const Path& path);
        void Save(const Path& path);
        bool IsActive() const;
        bool IsPaused() const;

        inline World* GetWorld() const { return mWorld.get(); }
       
    protected:
        String mName;
        UniquePtr<World> mWorld;
    };
}