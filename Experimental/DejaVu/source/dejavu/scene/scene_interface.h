#pragma once

#include <filesystem>
#include "utils/preprocess.h"
#include "scene/component.h"
#include "scene/entity.h"

namespace djv
{
    class DJV_API SceneInterface
    {
    public:
        
    };

    std::shared_ptr<SceneInterface> DJV_API createScene(const std::filesystem::path& path);
}