#ifdef PLATFORM_WINDOWS
#include "WindowsInputManager.h"
#include "Configuration/Windows/InputConfig.h"
#include "Window/Windows/Window.h"
#include "GLFW_InputCallbacks.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <Windows.h>

namespace Wanli
{
    LRESULT CALLBACK WindowsInputManager::InputMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return 0;
    }

    WindowsInputManager::~WindowsInputManager()
    {
        Destroy();
    }

    void WindowsInputManager::Initialize()
    {
        const auto& config = *InputConfig::Get();
        GLFWwindow* windowGLFW = config.GetWindow()->GetGLFW();
        HWND hwnd = config.GetWindow()->GetHwnd();

        glfwSetKeyCallback(windowGLFW, OnKey_GLFW);
        glfwSetMouseButtonCallback(windowGLFW, OnMouseButton_GLFW);
        glfwSetScrollCallback(windowGLFW, OnMouseScroll_GLFW);
        glfwSetCursorPosCallback(windowGLFW, OnCursorPos_GLFW);
        glfwSetCursorEnterCallback(windowGLFW, OnCursorEnter_GLFW);
        glfwSetDropCallback(windowGLFW, OnDrop_GLFW);

        config.GetWindow()->AddMessageHook(&WindowsInputManager::InputMessageProc);
    }

    void WindowsInputManager::Update()
    {}

    void WindowsInputManager::Destroy()
    {}
}

#endif