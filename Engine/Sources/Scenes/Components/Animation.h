#pragma once

#include "Scenes/IComponent.h"

namespace Wanli
{
    class Animation : public IComponent
    {
    public:
        using Function = std::function<void(float)>;

        Animation()
            : IComponent("Animation")
        {}
        virtual ~Animation() = default;

        virtual bool Initialize() override final { return true; }
        virtual void Update() override final {}

        inline void SetAnimationFunc(const Function& func) { mFunction = func; }
        inline const Function& GetAnimationFunc() const { return mFunction; }
        inline void Apply(float elapsedTime) { mFunction(elapsedTime); }

    protected:
        Function mFunction;
    };
}