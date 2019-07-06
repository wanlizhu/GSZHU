#pragma once
#include <ZHUENGINE/Common/Config.h>
#include <ZHUENGINE/Common/NonCopyable.h>

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