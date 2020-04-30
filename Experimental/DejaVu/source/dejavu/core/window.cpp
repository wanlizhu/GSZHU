#include "window.h"
#include "inputs.h"
#include "utils/logger.h"
#include "utils/strings.h"

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace djv
{
    static WNDPROC glfwMessageProc = nullptr;

    LRESULT CALLBACK WindowMessageProc(
        HWND hwnd,
        UINT msg, 
        WPARAM wParam, 
        LPARAM lParam
    )
    {
        Window* window = (Window*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
        if (window)
        {
            auto callbacks = window->getCallbacks();
            if (msg == WM_PAINT && callbacks)
            {
                callbacks->windowRedraw();                
            }
        }

        return glfwMessageProc(hwnd, msg, wParam, lParam);
    }

    Window::Window(
        const wchar_t* title, 
        int width, 
        int height, 
        std::weak_ptr<WindowCallbacks> callbacks, 
        WindowFlag flags
    )
    {
        glfwSetErrorCallback([](int code,const char* msg){
            LOG(ERROR) << "GLFW error: " << code << ": " << msg;
        });

        if (glfwInit() == GLFW_FALSE) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

#ifdef DJV_ENABLE_VULKAN
        if (glfwVulkanSupported() == GLFW_FALSE) {
            throw std::runtime_error("System doesn't support Vulkan API");
        }
#endif

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_STEREO, GLFW_FALSE);

        if (width <= 0 || height <= 0) {
            const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            width  = (int)(videoMode->width / 1.414);
            height = (int)(videoMode->height / 1.414);
        }

        std::string cstrTitle = to_string(title);
        GLFWwindow* window = glfwCreateWindow(width, height, cstrTitle.c_str(), nullptr, nullptr);
        mGLFWWindow = std::shared_ptr<GLFWwindow>(window, [=](GLFWwindow* handle) { 
            if (handle) {
                glfwDestroyWindow(handle);
                glfwTerminate();
            }
        });

        if (mGLFWWindow == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        HWND hwnd = glfwGetWin32Window(mGLFWWindow.get());
        glfwMessageProc = (WNDPROC)GetWindowLongPtrW(hwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowMessageProc);

        glfwSetWindowAttrib(mGLFWWindow.get(), GLFW_DECORATED, !is_set(flags, WindowFlag::Borderless));
        glfwSetWindowAttrib(mGLFWWindow.get(), GLFW_RESIZABLE, is_set(flags, WindowFlag::Resizable));
        glfwSetWindowAttrib(mGLFWWindow.get(), GLFW_FLOATING, is_set(flags, WindowFlag::Floating));
    
        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int x = (videoMode->width - width) / 2;
        int y = (videoMode->height - height) / 2;
        glfwSetWindowPos(mGLFWWindow.get(), x, y);

        if (is_set(flags, WindowFlag::Fullscreen)) {
            setFullscreen(true);
        }

        glfwSetFramebufferSizeCallback(mGLFWWindow.get(), 
        [](GLFWwindow* handle, int width,int height) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                callbacks->windowSizeChanged(width, height);
            }
        });

        glfwSetWindowCloseCallback(mGLFWWindow.get(), 
        [](GLFWwindow* handle) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                callbacks->windowClose();
            }
        });

        glfwSetKeyCallback(mGLFWWindow.get(), 
        [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                KeyboardEvent event(key, scancode, action, mods);
                callbacks->keyboardEvent(event);
            }
        });

        glfwSetMouseButtonCallback(mGLFWWindow.get(),
        [](GLFWwindow* handle, int button, int action, int mods) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(button, action, mods);
                callbacks->mouseEvent(event);
            }
        });

        glfwSetCursorPosCallback(mGLFWWindow.get(),
        [](GLFWwindow* handle, double x, double y) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(x, y, MOUSE_MOVE_TAG());
                callbacks->mouseEvent(event);
            }
        });

        glfwSetScrollCallback(mGLFWWindow.get(),
        [](GLFWwindow* handle, double x, double y) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(x, y, MOUSE_SCROLL_TAG());
                callbacks->mouseEvent(event);
            }
        });

        glfwSetDropCallback(mGLFWWindow.get(), 
        [](GLFWwindow* handle, int count,const char* paths[]){
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(count, paths);
                callbacks->mouseEvent(event);
            }
        });
    }

    void Window::messageLoop()
    {}

    void Window::close()
    {}
    
    void Window::resize(int width, int height)
    {}

    void Window::pollEvents()
    {}

    void Window::setFullscreen(bool enable)
    {}

    void Window::setWindowPos(int x, int y)
    {}

    void Window::setWindowTitle(const wchar_t* title)
    {}

    void Window::setCallbacks(std::weak_ptr<WindowCallbacks> callbacks)
    {}

    const wchar_t* Window::getWindowTitle() const
    {
        return mWindowTitle.c_str();
    }

    WindowHandle Window::getWindowHandle() const
    {
        return 0;
    }

    std::array<int, 2> Window::getClientAreaSize() const
    {
        return { 0, 0 };
    }

    WindowCallbacks* Window::getCallbacks() const
    {
        return nullptr;
    }
}

#elif defined(__APPLE__)
#else
#endif