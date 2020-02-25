#include "Engine.h"
#include "Utils/Log.h"
#include "Window/Window.h"
#include "Inputs/InputManager.h"
#include "Events/EventManager.h"
#include "Graphics/Graphics.h"
#include "Files/Files.h"
#include "Timers/Timers.h"

namespace Wanli
{
    Engine* Engine::Get()
    {
        static Engine instance;
        return &instance;
    }

    int Engine::Run()
    {
        try
        {
            Initialize();
            mRunning = true;
            while (mRunning)
            {
                if (mApp)
                {
                    if (!mApp->IsInited())
                        mApp->Initialize();
                    mApp->Update();
                }

                UpdateStage(EModuleStage::Always);

                // Update
                if (mFrameRate.GetUpdateCounter().EvalTickCount())
                {
                    mFrameRate.GetUpdateCounter().BeginDelta();
                    UpdateStage(EModuleStage::PreUpdate);
                    UpdateStage(EModuleStage::Update);
                    UpdateStage(EModuleStage::PostUpdate);
                    mFrameRate.GetUpdateCounter().EndDelta();
                }

                if (mPrioritizeUpdateOverRender)
                {
                    if (mFrameRate.GetUpdateCounter().GetDeltaTime() >=
                        mFrameRate.GetUpdateCounter().GetInterval())
                    {
                        continue;
                    }
                }

                // Render
                if (mFrameRate.GetRenderCounter().EvalTickCount())
                {
                    mFrameRate.GetRenderCounter().BeginDelta();
                    UpdateStage(EModuleStage::Render);
                    mFrameRate.GetRenderCounter().EndDelta();
                }
            }
            Destroy();
        }
        catch (const std::runtime_error & e)
        {
            LOG_ERROR("!!! %s !!!\n", e.what());
            return 1;
        }

        return 0;
    }

    Future<int> Engine::AsyncRun()
    {
        return std::async(std::launch::async,
            [&]() {
                return this->Run();
            });
    }

    void Engine::SetApp(IApplication::UPTR&& app)
    {
        mApp = std::move(app);
    }

    void Engine::Quit()
    {
        mRunning = false;
    }

    bool Engine::IsRunning() const
    {
        return mRunning;
    }

    IApplication* Engine::GetApp() const
    {
        return mApp.get();
    }

    ArgList const& Engine::GetArgList() const
    {
        return mArgList;
    }

    Version Engine::GetVersion() const
    {
        return mVersion;
    }

    FrameRateController const& Engine::GetFrameRate() const
    {
        return mFrameRate;
    }

    FrameRateController& Engine::GetFrameRate()
    {
        return mFrameRate;
    }

    void Engine::Initialize()
    {
        const auto& config = *EngineConfig::Get();

        mArgList = config.GetArgList();
        mPrioritizeUpdateOverRender = config.GetPrioritizeUpdateOverRender();
        mFrameRate.SetFPSLimit(config.GetFPSLimit());
        mFrameRate.SetUPSLimit(config.GetUPSLimit());

        Window::Register();
        InputManager::Register();
        EventManager::Register();
        Graphics::Register();
        Files::Register();
        Timers::Register();
        
        for (const auto& [stage, module] : IModule::GetRegistry())
        {
            module->Initialize();
        }
    }

    void Engine::Destroy()
    {
        for (const auto& [stage, module] : IModule::GetRegistry())
        {
            module->Destroy();
        }
        IModule::GetRegistry().clear();
    }

    void Engine::UpdateStage(EModuleStage targetStage)
    {
        for (const auto& [stage, module] : IModule::GetRegistry())
        {
            if (stage == targetStage && module)
            {
                module->Update();
            }
        }
    }
}