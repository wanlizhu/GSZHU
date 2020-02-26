#include "Scene.h"
#include "Utils/Log.h"

namespace Wanli
{
    void Scene::Close()
    {
        LOG_DEBUG("[Scene] %s Close\n", mName.c_str());
    }

    bool Scene::IsActive() const
    {
        return true;
    }

    bool Scene::IsPaused() const
    {
        return false;
    }

    bool Scene::Load(const Path& path)
    {
        return true;
    }

    void Scene::Save(const Path& path)
    {}
}