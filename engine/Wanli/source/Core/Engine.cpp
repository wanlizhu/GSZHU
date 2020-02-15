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

}