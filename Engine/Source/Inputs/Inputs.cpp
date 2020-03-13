#include "Inputs/Inputs.h"
#include "Utils/Log.h"
#include "Configuration/Windows/InputConfig.h"

#ifndef __ANDROID__
#include "Windows/WindowsInputManager.h"
#endif

namespace Wanli
{
    Inputs::Inputs()
    {
#ifndef __ANDROID__
        mImpl.reset(new WindowsInputManager());
#elif defined(__ANDROID__)
        mImpl.reset(new AndroidInputManager());
#endif
    }

    Inputs::~Inputs()
    {
        LOG_DEBUG("[IModule] Inputs Destroy\n");
    }

    void Inputs::Update()
    {
        mImpl->Update();
    }
}