#include "Engine/Common/GUI/Window.h"
#include <cassert>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace ZHU
{
    static bool __GLFW_inited = false;
    static void __GLFW_OnError(int code, const char* str)
    {
        printf("error %d:%s\n", code, str);
    }

    Window::Window()
    {
        Initialize();
    }

    Window::~Window()
    {
        Destroy();
    }

    Window::Window(const CreationInfo& info)
        : Window() // Constructor Delegation
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (info.fullscreen) {
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            mpWindow = glfwCreateWindow(mode->width, mode->height, info.title, monitor, NULL);
            assert(mpWindow != nullptr);
            return;
         }
        
        mpWindow = glfwCreateWindow(info.size[0], info.size[1], info.title, NULL, NULL);
        assert(mpWindow != nullptr);

        glfwSetWindowPos(mpWindow, info.pos[0], info.pos[1]);
    }

    INT2 Window::DefaultPos()
    {
        Initialize();
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        const INT2& size = DefaultSize();

        return INT2{ (mode->width - size[0]) / 2 , (mode->height - size[1]) / 2 };
    }

    INT2 Window::DefaultSize()
    {
        Initialize();
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        return INT2{ int(mode->width / 1.414), int(mode->height / 1.414) };
    }

    void Window::Initialize()
    {
        if (__GLFW_inited)
            return;

        glfwInit();
        glfwSetErrorCallback(__GLFW_OnError);

        __GLFW_inited = true;
    }

    void Window::MainLoop()
    {
        while (!glfwWindowShouldClose(mpWindow)) {

            glfwSwapBuffers(mpWindow);
            glfwPollEvents();
        }
    }

    void Window::Hide()
    {
        glfwHideWindow(mpWindow);
    }

    void Window::Show()
    {
        glfwShowWindow(mpWindow);
    }

    void Window::Destroy()
    {
        if (mpWindow != nullptr) {
            glfwDestroyWindow(mpWindow);
            mpWindow = nullptr;
        }
    }

    void Window::SetTitle(PCCH title)
    {
        glfwSetWindowTitle(mpWindow, title);
    }

    void Window::SetFullScreen(bool enabled)
    {
        static INT4 _normalWindow{ 0, 0, 0, 0 };

        if (enabled) {
            glfwGetWindowPos(mpWindow, &_normalWindow[0], &_normalWindow[1]);
            glfwGetWindowSize(mpWindow, &_normalWindow[2], &_normalWindow[3]);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(mpWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else {
            if (glfwGetWindowMonitor(mpWindow) != nullptr) {
                glfwSetWindowMonitor(mpWindow, NULL, _normalWindow[0], _normalWindow[1], _normalWindow[2], _normalWindow[3], 0);
            }
        }
    }

    PCCH Window::GetTitle() const
    {
        static std::string _title(MAX_PATH, '\0');
        HWND hwnd = glfwGetWin32Window(mpWindow);
        ::GetWindowTextA(hwnd, _title.data(), MAX_PATH);

        return _title.c_str();
    }

    INT2 Window::GetPosition() const
    {
        INT2 pos;
        glfwGetWindowPos(mpWindow, &pos[0], &pos[1]);
        return pos;
    }

    INT2 Window::GetSize() const
    {
        INT2 size;
        glfwGetWindowSize(mpWindow, &size[0], &size[1]);
        return size;
    }

    void Window::SetSize(int width, int height)
    {
        glfwSetWindowSize(mpWindow, width, height);
    }

    void Window::MoveTo(int x, int y)
    {
        glfwSetWindowPos(mpWindow, x, y);
    }
}