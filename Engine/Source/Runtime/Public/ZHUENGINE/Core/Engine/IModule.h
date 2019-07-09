#pragma once
#include <ZHUENGINE/Core/Config.h>
#include <ZHUENGINE/Core/NonCopyable.h>

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