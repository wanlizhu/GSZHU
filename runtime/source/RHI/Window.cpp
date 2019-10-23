#include "GE2/RHI/Window.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace GE2::RHI
{
    static void ErrorCallback(int code, const char* desc) 
    {
        printf("error: @GLFW %d:%s \n", code, desc);
    }

    static void WindowSizeCallback(GLFWwindow* _window, int w, int h)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);
        window->OnResize(w, h);
    }

    static void KeyCallback(GLFWwindow* _window, int key, int code, int action, int mods)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);

        KeyEvent event;
        if (action == GLFW_REPEAT || action == GLFW_KEY_UNKNOWN)
            return;

        event.type = (action == GLFW_RELEASE ? EKeyEventType::KeyUp : EKeyEventType::KeyDown);
        event.key = key;
        event.mods.shift = BIT_TEST(mods, GLFW_MOD_SHIFT);
        event.mods.ctrl = BIT_TEST(mods, GLFW_MOD_CONTROL);
        event.mods.alt = BIT_TEST(mods, GLFW_MOD_ALT);
        window->OnKeyEvent(event);
    }

    static void MouseButtonCallback(GLFWwindow* _window, int button, int action, int mods)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);

        MouseEvent event;
        if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS))          event.type = EMouseEventType::LeftDown;
        else if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_RELEASE))   event.type = EMouseEventType::LeftUp;
        else if ((button == GLFW_MOUSE_BUTTON_MIDDLE) && (action == GLFW_PRESS))   event.type = EMouseEventType::MiddleDown;
        else if ((button == GLFW_MOUSE_BUTTON_MIDDLE) && (action == GLFW_RELEASE)) event.type = EMouseEventType::MiddleUp;
        else if ((button == GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_PRESS))    event.type = EMouseEventType::RightDown;
        else if ((button == GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_RELEASE))  event.type = EMouseEventType::RightUp;
        else assert(false);

        event.mods.shift = BIT_TEST(mods, GLFW_MOD_SHIFT);
        event.mods.ctrl = BIT_TEST(mods, GLFW_MOD_CONTROL);
        event.mods.alt = BIT_TEST(mods, GLFW_MOD_ALT);

        double x, y;
        glfwGetCursorPos(_window, &x, &y);
        event.pos[0] = (int)x;
        event.pos[1] = (int)y;

        window->OnMouseEvent(event);
    }

    static void CursorPosCallback(GLFWwindow* _window, double x, double y)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);

        MouseEvent event;
        event.type = EMouseEventType::Move;
        
        double xpos, ypos;
        glfwGetCursorPos(_window, &xpos, &ypos);
        event.pos[0] = (int)xpos;
        event.pos[1] = (int)ypos;

        window->OnMouseEvent(event);
    }

    static void ScrollCallback(GLFWwindow* _window, double x, double y)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);

        MouseEvent event;
        event.type = EMouseEventType::Wheel;
        event.wheelDelta[0] = (float)x;
        event.wheelDelta[1] = (float)y;

        double xpos, ypos;
        glfwGetCursorPos(_window, &xpos, &ypos);
        event.pos[0] = (int)xpos;
        event.pos[1] = (int)ypos;

        window->OnMouseEvent(event);
    }

    static void CharCallback(GLFWwindow* _window, uint32_t input)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);

        KeyEvent event;
        event.type = EKeyEventType::Char;
        event.codepoint = input;

        window->OnKeyEvent(event);
    }

    static void DropCallback(GLFWwindow* _window, int count, const char** paths)
    {
        Window* window = (Window*)glfwGetWindowUserPointer(_window);
        assert(window != nullptr);

        STRLIST list;
        list.reserve(count);
        for (int i = 0; i < count; ++i)
        {
            list.push_back(paths[i]);
        }
        window->OnDropFile(list);
    }



    Window::Window()
    {}

    Window::~Window()
    {
        if (mpWindow)
        {
            glfwDestroyWindow(mpWindow);
            glfwTerminate();
            mpWindow = nullptr;
        }
    }

    bool Window::Create(const char* title, int w, int h, EWindowFlags flags)
    {
        if (glfwInit() == GLFW_FALSE)
        {
            printf("error @GLFW :failed to initialize");
            return false;
        }
        
        glfwSetErrorCallback(ErrorCallback);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        GLFWmonitor* monitor = EBIT_TEST(flags, EWindowFlags::Fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
        GLFWwindow* window = glfwCreateWindow(w, h, title, monitor, NULL);

        if (window == nullptr) 
        {
            printf("error @GLFW :failed to create window");
            return false;
        }

        mpWindow = window;
        glfwSetWindowUserPointer(mpWindow, this);
        glfwSetWindowSizeCallback(mpWindow, WindowSizeCallback);
        glfwSetKeyCallback(mpWindow, KeyCallback);
        glfwSetMouseButtonCallback(mpWindow, MouseButtonCallback);
        glfwSetCursorPosCallback(mpWindow, CursorPosCallback);
        glfwSetScrollCallback(mpWindow, ScrollCallback);
        glfwSetCharCallback(mpWindow, CharCallback);
        glfwSetDropCallback(mpWindow, DropCallback);

        return true;
    }

    void Window::Close() const
    {
        glfwSetWindowShouldClose(mpWindow, 1);
    }

    void Window::SetVisible(bool value) const
    {
        if (value)
            glfwShowWindow(mpWindow);
        else
            glfwHideWindow(mpWindow);
    }

    void Window::SetSize(int w, int h) const
    {
        glfwSetWindowSize(mpWindow, w, h);
    }

    void Window::SetPos(int x, int y) const
    {
        glfwSetWindowPos(mpWindow, x, y);
    }

    void Window::SetTitle(const char* title) const
    {
        glfwSetWindowTitle(mpWindow, title);
    }

    void Window::SetUserPointer(void* pointer) const
    {
        glfwSetWindowUserPointer(mpWindow, pointer);
    }

    INT2  Window::GetSize() const
    {
        INT2 size;
        glfwGetWindowSize(mpWindow, &size[0], &size[1]);
        return size;
    }

    INT2  Window::GetPos() const
    {
        INT2 pos;
        glfwGetWindowPos(mpWindow, &pos[0], &pos[1]);
        return pos;
    }

    void* Window::GetHandle() const
    {
        return glfwGetWin32Window(mpWindow);
    }

    void* Window::GetUserPointer() const
    {
        return glfwGetWindowUserPointer(mpWindow);
    }

    STRLIST Window::GetRequiredExtensions() const
    {
        uint32_t count = 0;
        const char** names = glfwGetRequiredInstanceExtensions(&count);
        
        STRLIST extensions;
        for (int i = 0; i < count; i++)
            extensions.emplace_back(names[i]);
        return extensions;
    }

    void  Window::SendEvent(uint32_t event, void* data)
    {
        ::SendMessageA((HWND)GetHandle(), event, NULL, (LPARAM)data);
    }

    void  Window::InstallEventFilter(uint32_t event, const EventHandler& handler)
    {
        mEventFilters[event] = handler;
    }
}