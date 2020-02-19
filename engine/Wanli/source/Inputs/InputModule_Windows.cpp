#ifdef PLATFORM_WINDOWS

#include "Inputs/InputModule.h"
#include "Application/WindowsApplication.h"
#include "Inputs/Windows/GLFWCallbacks.h"
#include "GLFW/glfw3.h"

namespace Wanli
{
    void InputModule::Initialize(IConfig* config)
    {
        InputConfig* inputConfig = static_cast<InputConfig*>(config);
        WindowsApplication* app = static_cast<WindowsApplication*>(inputConfig->mApp);
        GLFWwindow* window = app->GetWindowGLFW();

        glfwSetKeyCallback(window, OnKey_GLFW);
        glfwSetCharCallback(window, OnChar_GLFW);
        glfwSetMouseButtonCallback(window, OnMouseButton_GLFW);
        glfwSetScrollCallback(window, OnMouseScroll_GLFW);
        glfwSetCursorPosCallback(window, OnCursorPos_GLFW);
        glfwSetCursorEnterCallback(window, OnCursorEnter_GLFW);
    }

    void InputModule::Update(double elapsedTime)
    {}

    void InputModule::Shutdown()
    {}
}
#endif