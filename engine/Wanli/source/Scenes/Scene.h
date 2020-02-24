#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Utilities/Delegate.h"
#include "Utilities/NonCopyable.h"
#include "Helpers/Frustum.h"
#include "Scenes/Components/Camera.h"
#include "Scenes/IWorld.h"

namespace Wanli
{
    class SceneModule : public IModule
        , virtual public Observer
    {
    public:
        using ConfigType = SceneConfig;

        explicit SceneModule(const String& name)
            : IModule(EModuleStage::Update, "SceneModule")
            , mName(name)
        {}

        virtual ~SceneModule() = default;
        virtual void Initialize(IConfig* config) override final;
        virtual void Update(double elapsedTime) override final;
        virtual void Shutdown() override final;
        virtual bool IsActive() const;
        virtual bool IsPaused() const;

        inline void SetActiveCamera(Camera* camera) { mActiveCamera = camera; }
        inline Camera* GetCamera() const { return mActiveCamera; }
        inline IWorld* GetWorld() const { return mWorld.get(); }
       
    protected:
        String mName;
        Camera* mActiveCamera;
        UniquePtr<IWorld> mWorld;
    };
}