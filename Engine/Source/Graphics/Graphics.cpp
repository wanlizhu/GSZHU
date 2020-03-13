#include "Graphics.h"
#include "Utils/Log.h"
#include "Configuration/Windows/GraphicsConfig.h"

namespace Wanli
{
    Graphics::Graphics()
    {}

    Graphics::~Graphics()
    {
        LOG_DEBUG("[IModule] Graphics Destroy\n");
    }

    void Graphics::Update()
    {}
}