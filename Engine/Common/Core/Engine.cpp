#include "engine_pch.h"
#include <Common/Core/Engine.h>
#include <Common/Core/Timers.h>
#include <Common/Utilities/Log.h>

namespace ZHU
{
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

    IRenderer* Engine::GetRenderer() const
    {
        return mRenderer.get();
    }

    void Engine::SetRenderer(std::unique_ptr<IRenderer>&& renderer)
    {
        mRenderer = std::move(renderer);
    }
}