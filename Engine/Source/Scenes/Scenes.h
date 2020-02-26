#pragma once

#include "Core/Common.h"
#include "Core/IModule.h"
#include "Utils/Delegate.h"
#include "Utils/NonCopyable.h"
#include "Utils/Frustum.h"
#include "Components/Camera.h"
#include "World.h"
#include "Scene.h"

namespace Wanli
{
    class WANLI_API Scenes 
        : public IModule::Registrar<Scenes, void, EModuleStage::Update>
    {
    public:
        Scenes();
        virtual ~Scenes();

        virtual void Update() override final;

        void SetScene(Scene::PTR scene);
        void Close();

    protected:
        Scene* mActiveScene = nullptr;
        Array<Scene::PTR> mScenes;
    };
}