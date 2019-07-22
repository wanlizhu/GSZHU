#include "Engine/GUI/Window.h"
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
    {}

    Window::~Window()
    {
        Destroy();
    }

    Window::Window(const char* title, const Vec2i& size, const Vec2i& pos, bool fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (fullscreen) {
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            mpWindow = glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);
            assert(mpWindow != nullptr);
            return;
         }

        int w = size.x() == SCREEN_HALF.x() ? (int)(mode->width / 1.414)  : size.x();
        int h = size.y() == SCREEN_HALF.y() ? (int)(mode->height / 1.414) : size.y();
        mpWindow = glfwCreateWindow(w, h, title, NULL, NULL);
        assert(mpWindow != nullptr);

        int x = pos.x() == SCREEN_CENTER.x() ? (mode->width - w) / 2  : pos.x();
        int y = pos.y() == SCREEN_CENTER.y() ? (mode->height - h) / 2 : pos.y();
        glfwSetWindowPos(mpWindow, x, y);
    }

    void Window::Initialize()
    {
        if (__GLFW_inited)
            return;

        glfwInit();
        glfwSetErrorCallback(__GLFW_OnError);

        __GLFW_inited = true;
    }

    void Window::SetCreationHint(int hint, int value)
    {
        glfwWindowHint(hint, value);
    }

    void Window::MainLoop()
    {
        while (!glfwWindowShouldClose(mpWindow)) {

            glfwSwapBuffers(mpWindow);
            glfwPollEvents();
        }
    }

    void Window::Destroy()
    {
        if (mpWindow != nullptr) {
            glfwDestroyWindow(mpWindow);
            mpWindow = nullptr;
        }
    }

    void Window::SetFullScreen(bool enabled)
    {
        static Vec4i _GLFW_rect(0, 0, 1000, 618);

        if (enabled) {
            glfwGetWindowPos(mpWindow, &_GLFW_rect[0], &_GLFW_rect[1]);
            glfwGetWindowSize(mpWindow, &_GLFW_rect[2], &_GLFW_rect[3]);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(mpWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else {
            glfwSetWindowMonitor(mpWindow, NULL, _GLFW_rect[0], _GLFW_rect[1], _GLFW_rect[2], _GLFW_rect[3], 0);
        }
    }

    Vec2i Window::GetPosition() const
    {
        Vec2i pos;
        glfwGetWindowPos(mpWindow, &pos[0], &pos[1]);
        return pos;
    }

    Vec2i Window::GetSize() const
    {
        Vec2i size;
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

    GLFWwindow* Window::GetID() const
    {
        return mpWindow;
    }

    void Window::SetAttrib(int attrib, int value)
    {
        glfwSetWindowAttrib(mpWindow, attrib, value);
    }
}