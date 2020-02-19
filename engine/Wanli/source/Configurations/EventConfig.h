#pragma once

#include "IConfig.h"

namespace Wanli
{
    class EventModule;

    class DLLDECL EventConfig : public IConfig
    {
        friend class EventModule;
    public:
        using Module = EventModule;

        explicit EventConfig(const Configuration& configManager)
            : IConfig(configManager)
        {}
    };
}