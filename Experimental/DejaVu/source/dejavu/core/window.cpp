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
    std::weak_ptr<Window> gpWindow;
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
        mpGLFWWindow = std::shared_ptr<GLFWwindow>(window, [=](GLFWwindow* handle) { 
            if (handle) {
                this->close();
                glfwDestroyWindow(handle);
                glfwTerminate();
            }
        });

        if (mpGLFWWindow == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        HWND hwnd = glfwGetWin32Window(mpGLFWWindow.get());
        glfwMessageProc = (WNDPROC)GetWindowLongPtrW(hwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowMessageProc);

        glfwSetWindowAttrib(mpGLFWWindow.get(), GLFW_DECORATED, !is_set(flags, WindowFlag::Borderless));
        glfwSetWindowAttrib(mpGLFWWindow.get(), GLFW_RESIZABLE, is_set(flags, WindowFlag::Resizable));
        glfwSetWindowAttrib(mpGLFWWindow.get(), GLFW_FLOATING, is_set(flags, WindowFlag::Floating));
    
        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int x = (videoMode->width - width) / 2;
        int y = (videoMode->height - height) / 2;
        glfwSetWindowPos(mpGLFWWindow.get(), x, y);

        glfwSetFramebufferSizeCallback(mpGLFWWindow.get(), 
        [](GLFWwindow* handle, int width,int height) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                callbacks->windowSizeChanged(width, height);
            }
        });

        glfwSetWindowCloseCallback(mpGLFWWindow.get(), 
        [](GLFWwindow* handle) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                callbacks->windowClose();
            }
        });

        glfwSetKeyCallback(mpGLFWWindow.get(), 
        [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                KeyboardEvent event(key, scancode, action, mods);
                callbacks->keyboardEvent(event);
            }
        });

        glfwSetMouseButtonCallback(mpGLFWWindow.get(),
        [](GLFWwindow* handle, int button, int action, int mods) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(button, action, mods);
                callbacks->mouseEvent(event);
            }
        });

        glfwSetCursorPosCallback(mpGLFWWindow.get(),
        [](GLFWwindow* handle, double x, double y) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(x, y, MOUSE_MOVE_TAG());
                callbacks->mouseEvent(event);
            }
        });

        glfwSetScrollCallback(mpGLFWWindow.get(),
        [](GLFWwindow* handle, double x, double y) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(x, y, MOUSE_SCROLL_TAG());
                callbacks->mouseEvent(event);
            }
        });

        glfwSetDropCallback(mpGLFWWindow.get(), 
        [](GLFWwindow* handle, int count,const char* paths[]){
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            auto callbacks = window->getCallbacks();
            if (callbacks) {
                MouseEvent event(count, paths);
                callbacks->mouseEvent(event);
            }
        });

        gpWindow = shared_from_this();
    }

    void Window::messageLoop()
    {
        glfwShowWindow(mpGLFWWindow.get());
        glfwFocusWindow(mpGLFWWindow.get());

        while (!glfwWindowShouldClose(mpGLFWWindow.get()))
        {
            glfwWaitEvents();
        }
    }

    void Window::close()
    {
        mTickTimer.stop();
        glfwSetWindowShouldClose(mpGLFWWindow.get(), GLFW_TRUE);
    }
    
    void Window::resize(int width, int height)
    {
        assert(width > 0 && height > 0);
        glfwSetWindowSize(mpGLFWWindow.get(), width, height);

        // In minimized mode GLFW reports incorrect window size
        if (IsIconic(getWindowHandle())) {
            mMouseScale[0] = 1.0f / (float)width;
            mMouseScale[1] = 1.0f / (float)height;
        } else {
            glfwGetWindowSize(mpGLFWWindow.get(), &width, &height);
            mMouseScale[0] = 1.0f / (float)width;
            mMouseScale[1] = 1.0f / (float)height;
        }
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    void Window::setWindowPos(int x, int y)
    {
        glfwSetWindowPos(mpGLFWWindow.get(), x, y);
    }

    void Window::setWindowTitle(const wchar_t* title)
    {
        std::string cstrTitle = to_string(title);
        glfwSetWindowTitle(mpGLFWWindow.get(), cstrTitle.c_str());
    }

    void Window::setCallbacks(std::weak_ptr<WindowCallbacks> callbacks)
    {
        mpCallbacks = callbacks;
    }

    void Window::setTickTimer(std::chrono::milliseconds ms)
    {
        mTickTimer.set_interval(ms);
        mTickTimer.set_repeat(true);
        mTickTimer.set_callback([&](){
            RedrawWindow(
                getWindowHandle(), 
                NULL, /* [optional] RECT structure containing the coordinates of the update rectangle */
                NULL, /* [optional] Identifies the update region */
                RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_UPDATENOW
            );
        });
        mTickTimer.start();
    }

    const wchar_t* Window::getWindowTitle() const
    {
        return mWindowTitle.c_str();
    }

    WindowHandle Window::getWindowHandle() const
    {
        return glfwGetWin32Window(mpGLFWWindow.get());
    }

    std::array<int, 2> Window::getClientAreaSize() const
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(mpGLFWWindow.get(), &width, &height);
        return std::array<int, 2>{ width, height };
    }

    WindowCallbacks* Window::getCallbacks() const
    {
        return mpCallbacks.lock().get();
    }
}

#elif defined(__APPLE__)
#else
#endif