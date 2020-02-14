#include "Engine.h"

namespace Wanli
{
    Engine* Engine::Get()
    {
        static Engine instance;
        return &instance;
    }

    Engine::Engine()
    {}

    Engine::~Engine()
    {}

    bool Engine::Initialize(CommandLineArgs commandLineArgs,
                            WindowHandle window,
                            EDeviceType deviceType,
                            EFeatureFlags enabledFeatures,
                            Color backgroundColor,
                            const Array<Path>& assertPaths,
                            ErrorCallback errorCallback)
    {
        return true;
    }

}