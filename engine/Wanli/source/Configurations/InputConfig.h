#pragma once

#include "Configurations/IConfig.h"

namespace Wanli
{
    class IApplication;
    class InputModule;

    class DLLDECL InputConfig : public IConfig
    {
        friend class InputModule;
    public:
        using Module = InputModule;

        explicit InputConfig(const Configuration& configManager)
            : IConfig(configManager)
        {}

        InputConfig& SetApp(IApplication* app);

    private:
        IApplication* mApp = nullptr;
    };
}