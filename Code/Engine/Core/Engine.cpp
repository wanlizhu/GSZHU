#include "ZHUENGINE_PCH.h"
#include "Engine.h"
#include "Timers.h"

namespace ZHU
{
    Engine::Version Engine::smVersion;
    std::shared_ptr<Engine> Engine::smInstance(nullptr);

    bool Engine::Initialize()
    {
        Engine* instance = Get();
        instance->AddModule<Timers>(IModule::EStage::Global);

        return true;
    }

    Engine::Engine()
    {}

    Engine* Engine::Get()
    {
        if (smInstance == nullptr) {
            smInstance = std::shared_ptr<Engine>(new Engine());
            LOG_FATAL_IF(smInstance == nullptr, "!!! Failed to create Engine instance");
        }

        return smInstance.get();
    }

    void Engine::Shutdown()
    {

    }

    int Engine::Run() 
    {
        return 0;
    }

    const Engine::Version& Engine::GetVersion() const
    {
        return smVersion;
    }

    IRenderer* Engine::GetRenderer() const
    {
        return mRenderer.get();
    }

    void Engine::SetRenderer(std::unique_ptr<IRenderer>&& renderer)
    {
        mRenderer = std::move(renderer);
    }

    int Engine::GetFPS() const
    {
        return mFPS.GetPFS();
    }
}