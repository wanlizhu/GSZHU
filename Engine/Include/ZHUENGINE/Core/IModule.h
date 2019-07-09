#pragma once
#include <ZHUENGINE/Config.h>
#include <ZHUENGINE/Core/Common/NonCopyable.h>

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