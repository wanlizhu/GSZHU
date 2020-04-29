#include "Window.h"
#include <glog/logging.h>
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
            if (msg == WM_PAINT && (auto callbacks = window->getWindowCallbacks()))
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
        WindowFlag flags = WindowFlag::None
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
            auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            width  = (int)(videoMode.width / 1.414);
            height = (int)(videoMode.height / 1.414);
        }

        std::string cstrTitle = to_string(title);
        auto window = glfwCreateWindow(width, height, cstrTitle.c_str(), nullptr, nullptr);
        mGLFWWindow = std::shared_ptr<GLFWwindow>(new GLFWwindow(window), [=](GLFWwindow* pointer) { 
            if (*pointer) {
                glfwDestroyWindow(*pointer);
                glfwTerminate();
            }
            delete pointer;
        });

        if (*mGLFWWindow == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        HWND hwnd = glfwGetWin32Window(*mGLFWWindow);
        glfwMessageProc = (WNDPROC)GetWindowLongPtrW(hwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, this);
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowMessageProc);

        glfwSetWindowAttrib(*mGLFWWindow, GLFW_DECORATED, !(flags & WindowFlag::Borderless);
        glfwSetWindowAttrib(*mGLFWWindow, GLFW_RESIZABLE, flags & WindowFlag::Resizable);
        glfwSetWindowAttrib(*mGLFWWindow, GLFW_FLOATING, flags & WindowFlag::Floating);
    
        int x = (videoMode.width - width) / 2;
        int y = (videoMode.height - height) / 2;
        glfwSetWindowPos(*mGLFWWindow, x, y);

        if (flags & WindowFlag::Fullscreen) {
            setFullscreen(true);
        }

        glfwSetFramebufferSizeCallback(*mGLFWWindow, [](GLFWwindow* handle, int width,int height) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            if (window && (auto callbacks = window->getCallbacks())) {
                callbacks->windowSizeChanged(width, height);
            }
        });

        glfwSetWindowCloseCallback(*mGLFWWindow, [](GLFWwindow* handle) {
            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            if (window && (auto callbacks = window->getCallbacks())) {
                callbacks->windowClose();
            }
        });


    }
}

#elif defined(__APPLE__)
#else
#endif