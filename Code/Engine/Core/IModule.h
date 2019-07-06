#pragma once
#include "Common/Config.h"
#include "Common/NonCopyable.h"

namespace ZHU
{
    class ZHU_API IModule : public NonCopyable
    {
    public:
        enum class EStage {
            Global,
            Render,
        };

        virtual ~IModule() = default;
        virtual void Update() = 0;
    };
}