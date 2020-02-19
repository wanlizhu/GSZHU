#pragma once

#include "Configurations/IConfig.h"

namespace Wanli
{
    class SceneModule;

    enum class EWorldType
    {
        LinkedList,
        RTree,
        BVH,
    };

    class DLLDECL SceneConfig : public IConfig
    {
        friend class SceneModule;
    public:
        using Module = SceneModule;

        explicit SceneConfig(const Configuration& configManager)
            : IConfig(configManager)
        {}

        SceneConfig& SetWorldType(EWorldType type);
        SceneConfig& LoadScene(const Path& path);

    protected:
        EWorldType mWorldType;
        Path mScenePath;
    };
}