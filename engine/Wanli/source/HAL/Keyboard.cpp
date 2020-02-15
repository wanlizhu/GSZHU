#include "HAL/Keyboard.h"
#include "Core/Engine.h"
#include "glfw/glfw3.h"

namespace Wanli
{
    class CallbacksGLFW
    {
    public:
        static void OnKey_GLFW(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
        {
            Engine::Get()->GetModule<Keyboard>()->OnKey(
                static_cast<EKey>(key),
                static_cast<EInputAction>(action),
                static_cast<EInputMods>(mods)
            );
        }

        static void OnChar_GLFW(GLFWwindow* window, uint32_t codepoint)
        {
            Engine::Get()->GetModule<Keyboard>()->OnChar(codepoint);
        }
    };

    Keyboard::Keyboard(GLFWwindow* window)
        : IModule(EModuleStage::PreUpdate, "Keyboard")
        , mWindowGLFW(window)
    {
        glfwSetKeyCallback(mWindowGLFW, &CallbacksGLFW::OnKey_GLFW);
        glfwSetCharCallback(mWindowGLFW, &CallbacksGLFW::OnChar_GLFW);
    }

    void Keyboard::Update()
    {}

    EInputAction Keyboard::GetKey(EKey key) const
    {
        auto state = glfwGetKey(mWindowGLFW, static_cast<int32_t>(key));
        return static_cast<EInputAction>(state);
    }
}