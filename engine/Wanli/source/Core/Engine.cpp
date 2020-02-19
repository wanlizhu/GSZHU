#include "Engine.h"
#include "Utilities/Traits.h"

#include "Application/IApplication.h"
#include "Graphics/GraphicsModule.h"
#include "Physics/PhysicsModule.h"
#include "Inputs/InputModule.h"
#include "Events/EventModule.h"
#include "Scenes/SceneModule.h"

namespace Wanli
{
    String gszLastErrorMsg;

    Engine* Engine::Get()
    {
        static Engine instance;
        return &instance;
    }

    void Engine::Initialize()
    {
        mConfig.SealUp();
        THROW_IF_NOT(mApp != nullptr);

        Register<GraphicsModule>();
        Register<PhysicsModule>();
        Register<InputModule>();
        Register<EventModule>();
        Register<SceneModule>("Scene1");
    }

    void Engine::Shutdown()
    {
        for (auto& pair : mModules)
        {
            pair.second->Shutdown();
        }
        mApp->Shutdown();

        mApp = nullptr;
        mModules.clear();
    }

    void Engine::UpdateStage(EModuleStage stage, double elapsedTime)
    {
        for (const auto& [type, module] : mModules)
        {
            if (module && module->GetModuleStage() == stage)
            {
                module->Update(elapsedTime);
            }
        }
    }

    int Engine::Run()
    {
        try
        {
            Initialize();

            while (!mApp->IsQuit())
            {
                mFPSCounter.BeginTick();
                const auto& deltaMs = mFPSCounter.GetElapsedTimeMs();
                
                mApp->Update();
                UpdateStage(EModuleStage::PreUpdate, deltaMs);
                UpdateStage(EModuleStage::Update, deltaMs);
                UpdateStage(EModuleStage::Render, deltaMs);
                UpdateStage(EModuleStage::PostRender, deltaMs);
                
                mFPSCounter.EndTick();
            }

            Shutdown();
        }
        catch (const std::runtime_error& e)
        {
            LOG_ERROR("!!! %s !!!\n", e.what());
            return 1;
        }

        return 0;
    }

    Future<int> Engine::RunAsync()
    {
        return std::async(std::launch::async,
                          [&] () {
                              return this->Run();
                          });
    }
}