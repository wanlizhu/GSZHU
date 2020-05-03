#include "scene_impl.h"

namespace djv
{
    std::weak_ptr<SceneInterface> gpScene;
    
    std::shared_ptr<SceneInterface> createScene(const std::filesystem::path& path)
    {
        return nullptr;
    }
}