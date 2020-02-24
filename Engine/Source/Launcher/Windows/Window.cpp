#ifdef PLATFORM_WINDOWS

#include "Launcher/Windows/Window.h"
#include "Launcher/Windows/GLFWCallbacks.h"
#include "Utils/Log.h"
#include "Utils/Bitmap.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <Windows.h>

namespace Wanli
{
    Window::Window(WindowCreateInfo& info_)
    {
        glfwSetErrorCallback(OnError_GLFW);
        if (glfwInit() == GLFW_FALSE)
        {
            throw std::runtime_error("GLFW failed to initialize");
        }

#ifdef ENABLE_VULKAN
        if (glfwVulkanSupported() == GLFW_FALSE)
        {
            throw std::runtime_error("GLFW failed to find Vulkan support");
        }
#endif

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_STEREO, GLFW_FALSE);

        mMonitor.SetMonitor(glfwGetPrimaryMonitor());
        auto videoMode = mMonitor.GetVideoMode();

        if (info.size.x <= 0 || info.size.y <= 0)
        {
            info.size.x = (int)(videoMode.width / 1.414);
            info.size.y = (int)(videoMode.height / 1.414);
        }

        mWindowGLFW = glfwCreateWindow(
            info.size.x,
            info.size.y,
            info.title.c_str(),
            nullptr,
            nullptr);
        if (mWindowGLFW == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("GLFW failed to create window");
        }

        if (info.createMenu)
        {
            CreateMenu();
        }

        glfwSetWindowUserPointer(mWindowGLFW, this);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, !info.attribs.borderless);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, info.attribs.resizable);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_FLOATING, info.attribs.floating);

        info.pos.x = info.pos.x < 0 ? ((videoMode.width - info.size.x) / 2) : info.pos.x;
        info.pos.y = info.pos.y < 0 ? ((videoMode.height - info.size.y) / 2) : info.pos.y;
        glfwSetWindowPos(mWindowGLFW, info.pos.x, info.pos.y);

        if (info.attribs.fullscreen)
        {
            SetFullscreen(true);
        }

        glfwSetWindowPosCallback(mWindowGLFW, OnWindowPos_GLFW);
        glfwSetWindowSizeCallback(mWindowGLFW, OnWindowSize_GLFW);
        glfwSetWindowCloseCallback(mWindowGLFW, OnWindowClose_GLFW);
        glfwSetWindowFocusCallback(mWindowGLFW, OnWindowFocus_GLFW);
        glfwSetWindowIconifyCallback(mWindowGLFW, OnWindowIconified_GLFW);
        glfwSetFramebufferSizeCallback(mWindowGLFW, OnFramebufferSize_GLFW);

        glfwShowWindow(mWindowGLFW);
    }
}

#endif