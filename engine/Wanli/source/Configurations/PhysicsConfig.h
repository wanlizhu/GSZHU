#pragma once

#include "Configurations/IConfig.h"

namespace Wanli
{
    class PhysicsModule;

    class DLLDECL PhysicsConfig : public IConfig
    {
        friend class PhysicsModule;
    public:
        using Module = PhysicsModule;

        explicit PhysicsConfig(const Configuration& configManager)
            : IConfig(configManager)
        {}
    };
}