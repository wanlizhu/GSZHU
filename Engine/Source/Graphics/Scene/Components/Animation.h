#pragma once

#include "Graphics/Scene/IComponent.h"

namespace Wanli
{
    class WANLI_API Animation : public IComponent
    {
    public:
        using Function = std::function<void(float)>;

        Animation()
            : IComponent("Animation")
        {}
        virtual ~Animation() = default;

        virtual void Initialize() override final {}
        virtual void Update() override final {}

        inline void SetAnimationFunc(const Function& func) { mFunction = func; }
        inline Function const& GetAnimationFunc() const { return mFunction; }
        inline void Apply(float elapsedTime) { mFunction(elapsedTime); }

    protected:
        Function mFunction;
    };
}