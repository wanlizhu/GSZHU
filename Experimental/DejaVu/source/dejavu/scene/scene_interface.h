#pragma once

#include <filesystem>
#include "utils/preprocess.h"
#include "scene/component.h"
#include "scene/entity.h"

namespace djv
{
    class DJV_API SceneInterface : public std::enable_shared_from_this<SceneInterface>
    {
    public:
        
    };

    DJV_API extern std::weak_ptr<SceneInterface> gpScene;

    std::shared_ptr<SceneInterface> DJV_API createScene(const std::filesystem::path& path);
}