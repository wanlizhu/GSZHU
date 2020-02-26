#include "Timers.h"
#include "Utils/Log.h"

namespace Wanli
{
    Timers::Timers()
    {}

    Timers::~Timers()
    {
        LOG_DEBUG("[IModule] Timers Destroy\n");
    }

    void Timers::Update()
    {}
}