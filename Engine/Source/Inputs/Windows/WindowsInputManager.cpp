#ifndef __ANDROID__
#include "WindowsInputManager.h"
#include "Configuration/Windows/InputConfig.h"
#include "Window/Windows/Window.h"
#include "GLFW_InputCallbacks.h"
#include "Utils/Log.h"

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

    WindowsInputManager::WindowsInputManager()
    {
        const auto& config = *InputConfig::Get();
        GLFWwindow* windowGLFW = Window::Get()->GetGLFW();
        HWND hwnd = Window::Get()->GetHwnd();

        glfwSetKeyCallback(windowGLFW, OnKey_GLFW);
        glfwSetMouseButtonCallback(windowGLFW, OnMouseButton_GLFW);
        glfwSetScrollCallback(windowGLFW, OnMouseScroll_GLFW);
        glfwSetCursorPosCallback(windowGLFW, OnCursorPos_GLFW);
        glfwSetCursorEnterCallback(windowGLFW, OnCursorEnter_GLFW);
        glfwSetDropCallback(windowGLFW, OnDrop_GLFW);

        Window::Get()->AddMessageHook(&WindowsInputManager::InputMessageProc);
    }

    WindowsInputManager::~WindowsInputManager()
    {}

    void WindowsInputManager::Update()
    {}
}

#endif