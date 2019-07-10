#pragma once
#include <Engine/Config.h>
#include <memory>

namespace ZHU
{
    template<typename... ARGS>
    class IObserver
    {
    public:
        virtual void Update(ARGS... args) = 0;
    };
}