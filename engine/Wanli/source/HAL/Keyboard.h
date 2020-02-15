#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "HAL/UserInputFlags.h"
#include "Utilities/Delegate.h"

struct GLFWwindow;

namespace Wanli
{
    class DLLDECL Keyboard : public IModule
    {
        friend class CallbacksGLFW;
    public:
        Keyboard(GLFWwindow* window);
        virtual ~Keyboard() = default;

        virtual void Update() override;
        EInputAction GetKey(EKey key) const;

    public:
        Delegate<void(EKey, EInputAction, EInputMods)> OnKey;
        Delegate<void(Uint)> OnChar;

    private:
        GLFWwindow* mWindowGLFW = nullptr;
    };
}