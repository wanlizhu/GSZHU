#ifdef PLATFORM_WINDOWS

#include "Window/Windows/Window.h"
#include "Window/Windows/GLFW_WindowCallbacks.h"
#include "Utils/Log.h"
#include "Utils/Bitmap.h"
#include "Configuration/Windows/WindowConfig.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <Windows.h>

namespace Wanli
{
    static Window* local_windowInstance = nullptr;
    static WNDPROC local_messageProcGLFW = nullptr;

    LRESULT CALLBACK Window::WindowMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (local_windowInstance)
        {
            for (const auto& hookProc : local_windowInstance->mMessageHooks)
            {
                hookProc(hwnd, msg, wParam, lParam);
            }
        }

        return local_messageProcGLFW(hwnd, msg, wParam, lParam);
    }

    void Window::Initialize()
    {
        const auto& config = *WindowConfig::Get();

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

        glm::ivec2 size = config.GetSize();
        if (size.x <= 0 || size.y <= 0)
        {
            size.x = (int)(videoMode.width / 1.414);
            size.y = (int)(videoMode.height / 1.414);
        }

        mWindowGLFW = glfwCreateWindow(
            size.x,
            size.y,
            config.GetTitle().c_str(),
            nullptr,
            nullptr);
        if (mWindowGLFW == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("GLFW failed to create window");
        }

        mHwnd = glfwGetWin32Window(mWindowGLFW);
        local_windowInstance = this;
        local_messageProcGLFW = (WNDPROC)GetWindowLongPtrW(mHwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(mHwnd, GWLP_WNDPROC, (LONG_PTR)&Window::WindowMessageProc);
        CreateMenu();

        glfwSetWindowUserPointer(mWindowGLFW, this);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, !config.GetAttribs().borderless);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, config.GetAttribs().resizable);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_FLOATING, config.GetAttribs().floating);

        glm::ivec2 pos = config.GetPos();
        pos.x = pos.x < 0 ? ((videoMode.width - size.x) / 2) : pos.x;
        pos.y = pos.y < 0 ? ((videoMode.height - size.y) / 2) : pos.y;
        glfwSetWindowPos(mWindowGLFW, pos.x, pos.y);

        if (config.GetAttribs().fullscreen)
        {
            SetFullscreen(true);
        }

        glfwSetWindowPosCallback(mWindowGLFW, OnWindowPos_GLFW);
        glfwSetWindowSizeCallback(mWindowGLFW, OnWindowSize_GLFW);
        glfwSetWindowCloseCallback(mWindowGLFW, OnWindowClose_GLFW);
        glfwSetWindowIconifyCallback(mWindowGLFW, OnWindowIconified_GLFW);

        glfwShowWindow(mWindowGLFW);
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Update()
    {
        glfwPollEvents();
    }

    void Window::Destroy()
    {
        if (mWindowGLFW)
        {
            glfwDestroyWindow(mWindowGLFW);
            mWindowGLFW = nullptr;
            local_windowInstance = nullptr;
            glfwTerminate();
        }
    }

    void Window::Show()
    {
        glfwShowWindow(mWindowGLFW);
    }

    void Window::Hide()
    {
        glfwHideWindow(mWindowGLFW);
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(mWindowGLFW, true);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(mWindowGLFW);
    }

    GLFWwindow* Window::GetGLFW()
    {
        return mWindowGLFW;
    }

    Monitor& Window::GetMonitor()
    {
        return mMonitor;
    }

    Cursor& Window::GetCursor()
    {
        return mCursor;
    }

    Menu& Window::GetMenu()
    {
        return mMenu;
    }

    HWND Window::GetHwnd() const
    {
        return mHwnd;
    }

    glm::ivec2 Window::GetSize() const
    {
        glm::ivec2 size;
        glfwGetWindowSize(mWindowGLFW, &size.x, &size.y);
        return size;
    }

    glm::ivec2 Window::GetPos() const
    {
        glm::ivec2 size;
        glfwGetWindowPos(mWindowGLFW, &size.x, &size.y);
        return size;
    }

    int Window::GetAttribute(int attrib) const
    {
        return glfwGetWindowAttrib(mWindowGLFW, attrib);
    }

    
    void Window::SetSize(const glm::ivec2& size)
    {
        glfwSetWindowSize(mWindowGLFW, size.x, size.y);
    }

    void Window::SetPos(const glm::ivec2& pos)
    {
        glfwSetWindowPos(mWindowGLFW, pos.x, pos.y);
    }

    void Window::SetTitle(const char* name)
    {
        glfwSetWindowTitle(mWindowGLFW, name);
    }

    void Window::SetIcons(const Array<Path>& paths)
    {
        Array<UniquePtr<Bitmap>> bitmaps;
        Array<GLFWimage> icons;

        for (const auto& path : paths)
        {
            auto bitmap = std::make_unique<Bitmap>(path);
            if (!bitmap.get())
                continue;

            GLFWimage icon = {};
            icon.width = bitmap->GetSize().x;
            icon.height = bitmap->GetSize().y;
            icon.pixels = bitmap->GetData().get();
            icons.emplace_back(icon);
            bitmaps.emplace_back(std::move(bitmap));
        }

        glfwSetWindowIcon(mWindowGLFW, (int)icons.size(), icons.data());
    }

    void Window::SetFullscreen(bool value)
    {
        if (mFullscreen == value)
            return;

        mFullscreen = value;
        auto& selectedMonitor = GetMonitor();
        auto videoMode = selectedMonitor.GetVideoMode();

        if (value)
        {
            mRestorePos = GetPos();
            mRestoreSize = GetSize();
            glfwSetWindowMonitor(mWindowGLFW,
                selectedMonitor.GetMonitorGLFW(),
                0,
                0,
                videoMode.width,
                videoMode.height,
                GLFW_DONT_CARE);
        }
        else
        {
            glfwSetWindowMonitor(mWindowGLFW,
                nullptr,
                mRestorePos.x,
                mRestorePos.y,
                mRestoreSize.x,
                mRestoreSize.y,
                GLFW_DONT_CARE);
        }
    }

    void Window::SetAttribute(int attrib, int value)
    {
        glfwSetWindowAttrib(mWindowGLFW, attrib, value);
    }

    void Window::AddMessageHook(const MessageHook& hook)
    {
        mMessageHooks.emplace_back(hook);
    }

    void Window::CreateMenu()
    {
        mMessageHooks.emplace_back(&Menu::MenuMessageProc);

        mMenu.BeginHorizontalMenu(mHwnd);

        mMenu.BeginVerticalMenu("File");
        mMenu.AddMenuItem("New", EMenuID::File_New);
        mMenu.AddMenuItem("Open", EMenuID::File_Open);
        mMenu.AddMenuItem("Initialize", EMenuID::File_Create);
        mMenu.AddSeparator();
        mMenu.AddMenuItem("Save", EMenuID::File_Save);
        mMenu.AddMenuItem("Save As", EMenuID::File_SaveAs);
        mMenu.AddSeparator();
        mMenu.AddMenuItem("Edit Perference", EMenuID::File_EditPerference);
        mMenu.AddSeparator();
        mMenu.AddMenuItem("Exit", EMenuID::File_Exit);
        mMenu.EndVerticalMenu();

        mMenu.EndHorizontalMenu();
    }
}

#endif