#include "Scenes.h"
#include "Utils/Log.h"

namespace Wanli
{
    Scenes::Scenes()
    {}

    Scenes::~Scenes()
    {
        LOG_DEBUG("[IModule] Scenes Destroy\n");
        Close();
    }

    void Scenes::Update()
    {}

    void Scenes::SetScene(Scene::PTR scene)
    {
        mActiveScene = scene.get();
        mScenes.push_back(scene);
    }

    void Scenes::Close() 
    {
        SetScene(nullptr);
        mScenes.clear();
    }
}