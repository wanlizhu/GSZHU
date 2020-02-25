#include "Scene.h"

namespace Wanli
{
    void Scene::Initialize()
    {}

    void Scene::Update()
    {}

    void Scene::Destroy()
    {}

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