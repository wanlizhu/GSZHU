#include "Scenes/SceneModule.h"

namespace Wanli
{
    void SceneModule::Initialize(IConfig* config)
    {}

    void SceneModule::Update(double elapsedTime)
    {}

    void SceneModule::Shutdown()
    {}

    bool SceneModule::IsActive() const
    {
        return true;
    }

    bool SceneModule::IsPaused() const
    {
        return false;
    }
}