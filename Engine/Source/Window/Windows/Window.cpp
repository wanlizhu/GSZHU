#ifdef PLATFORM_WINDOWS

#include "Window/Windows/Window.h"
#include "Window/Windows/GLFW_Callbacks.h"
#include "Utils/Log.h"
#include "Utils/Bitmap.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <Windows.h>

namespace Wanli
{
    void Window::Create(WindowCreateInfo& info)
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
        glfwSetWindowIconifyCallback(mWindowGLFW, OnWindowIconified_GLFW);

        glfwShowWindow(mWindowGLFW);
    }

    Window::~Window()
    {
        if (mWindowGLFW)
        {
            glfwDestroyWindow(mWindowGLFW);
            mWindowGLFW = nullptr;
            glfwTerminate();
        }
    }

    void Window::Update()
    {
        glfwPollEvents();
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

    static WNDPROC glfwProc = nullptr;
    LRESULT CALLBACK windowProcNew(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return glfwProc(hwnd, msg, wParam, lParam);
    }

    void Window::CreateMenu()
    {
        HWND hwnd = glfwGetWin32Window(mWindowGLFW);
        glfwProc = (WNDPROC)GetWindowLongPtrW(hwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)windowProcNew);
    
        mMenu.BeginHorizontalMenu(hwnd);

        mMenu.BeginVerticalMenu("File");
        mMenu.AddMenuItem("New", EMenuID::File_New);
        mMenu.AddMenuItem("Open", EMenuID::File_Open);
        mMenu.AddMenuItem("Create", EMenuID::File_Create);
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