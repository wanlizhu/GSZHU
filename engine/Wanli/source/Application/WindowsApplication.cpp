#ifdef PLATFORM_WINDOWS
#include "Application/WindowsApplication.h"
#include "Application/Windows/GLFWCallbacks.h"
#include "Utilities/Log.h"
#include "Utilities/Bitmap.h"
#include "Core/Engine.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <Windows.h>

namespace Wanli
{
    WindowsApplication::WindowsApplication()
    {}

    WindowsApplication::~WindowsApplication()
    {
        Shutdown();
    }

    void WindowsApplication::Initialize(const WindowCreateInfo& info)
    {
        mInfo = info;

        glfwSetErrorCallback(OnError_GLFW);
        if (glfwInit() == GLFW_FALSE)
        {
            throw std::runtime_error("GLFW failed to initialize");
        }

        if (mInfo.supportAPI == ERHIDevice::Vulkan &&
            glfwVulkanSupported() == GLFW_FALSE)
        {
            throw std::runtime_error("GLFW failed to find Vulkan support");
        }

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_STEREO, GLFW_FALSE);

        mCurrentMonitor = std::move(Monitor(glfwGetPrimaryMonitor()));
        auto videoMode = mCurrentMonitor.GetVideoMode();

        if (mInfo.size.x <= 0 || mInfo.size.y <= 0)
        {
            mInfo.size.x = (int)(videoMode.width / 1.414);
            mInfo.size.y = (int)(videoMode.height / 1.414);
        }

        mWindowGLFW = glfwCreateWindow(
            mInfo.size.x,
            mInfo.size.y,
            mInfo.title.c_str(),
            nullptr,
            nullptr);
        if (mWindowGLFW == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("GLFW failed to create window");
        }

        if (mInfo.createMenu)
        {
            CreateMenu();
        }

        glfwSetWindowUserPointer(mWindowGLFW, this);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, !mInfo.borderless);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, mInfo.resizable);
        glfwSetWindowAttrib(mWindowGLFW, GLFW_FLOATING, mInfo.floating);

        mInfo.pos.x = mInfo.pos.x < 0 ? ((videoMode.width - mInfo.size.x) / 2) : mInfo.pos.x;
        mInfo.pos.y = mInfo.pos.y < 0 ? ((videoMode.height - mInfo.size.y) / 2) : mInfo.pos.y;
        glfwSetWindowPos(mWindowGLFW, mInfo.pos.x, mInfo.pos.y);

        if (mInfo.fullscreen)
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

    void WindowsApplication::Update()
    {
        glfwPollEvents();
    }

    void WindowsApplication::Shutdown()
    {
        if (mWindowGLFW != nullptr)
        {
            glfwDestroyWindow(mWindowGLFW);
            glfwTerminate();

            mWindowGLFW = nullptr;
        }
    }

    bool WindowsApplication::IsQuit() const
    {
        return glfwWindowShouldClose(mWindowGLFW);
    }

    WindowHandle WindowsApplication::GetWindowHandle() const
    {
        return (WindowHandle)glfwGetWin32Window(mWindowGLFW);
    }

    glm::ivec2 WindowsApplication::GetContentSize() const
    {
        return mInfo.fullscreen ? mFullscreenSize : mInfo.size;
    }

    glm::vec2 WindowsApplication::GetContentScale() const
    {
        return { 1.f, 1.f };
    }

    PCCHArray WindowsApplication::GetRequiredVulkanExtensions() const
    {
        return {};
    }

    PCCHArray WindowsApplication::GetRequiredVulkanLayers() const
    {
        return {};
    }

    GLFWwindow* WindowsApplication::GetWindowGLFW() const
    {
        return mWindowGLFW;
    }

    const Monitor& WindowsApplication::GetMonitor() const
    {
        return mCurrentMonitor;
    }

    glm::ivec2 WindowsApplication::GetPosition() const
    {
        return mInfo.pos;
    }

    void WindowsApplication::SetSize(const glm::ivec2& size)
    {
        mInfo.size = size;
        glfwSetWindowSize(mWindowGLFW, size.x, size.y);
    }

    void WindowsApplication::SetPosition(const glm::ivec2& pos)
    {
        mInfo.pos = pos;
        glfwSetWindowPos(mWindowGLFW, pos.x, pos.y);
    }

    void WindowsApplication::SetTite(const String& title)
    {
        mInfo.title = title;
        glfwSetWindowTitle(mWindowGLFW, title.c_str());
    }

    void WindowsApplication::SetIcons(const Array<Path>& paths)
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

    void WindowsApplication::SetIconified(bool iconify)
    {
        if (!mIconified && iconify)
        {
            glfwIconifyWindow(mWindowGLFW);
        }
        else if (mIconified && !iconify)
        {
            glfwRestoreWindow(mWindowGLFW);
        }
    }

    void WindowsApplication::SetFloating(bool floating)
    {
        mInfo.floating = floating;
        glfwSetWindowAttrib(mWindowGLFW, GLFW_FLOATING, floating);
    }

    void WindowsApplication::SetResizable(bool resizable)
    {
        mInfo.resizable = resizable;
        glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, resizable);
    }

    void WindowsApplication::SetBorderless(bool borderless)
    {
        mInfo.borderless = borderless;
        glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, !borderless);
    }
    
    void WindowsApplication::SetFullscreen(bool fullscreen, const Optional<Monitor>& monitor)
    {
        mInfo.fullscreen = fullscreen;

        auto selected = monitor.has_value() ? monitor.value() : GetMonitor();
        auto videoMode = selected.GetVideoMode();

        if (fullscreen)
        {
            mFullscreenSize = { videoMode.width, videoMode.height };
            glfwSetWindowMonitor(mWindowGLFW,
                selected.GetMonitorGLFW(),
                0,
                0,
                mFullscreenSize.x,
                mFullscreenSize.y,
                GLFW_DONT_CARE);
        }
        else
        {
            glfwSetWindowMonitor(mWindowGLFW,
                nullptr,
                mInfo.pos.x,
                mInfo.pos.y,
                mInfo.size.x,
                mInfo.size.y,
                GLFW_DONT_CARE);
        }
    }

    namespace local
    {
        static WNDPROC glfwProc = nullptr;
        LRESULT CALLBACK windowProcNew(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            Menu::MessageProc(hwnd, msg, wParam, lParam);
            return glfwProc(hwnd, msg, wParam, lParam);
        }
    }

    void WindowsApplication::CreateMenu()
    {
        HWND hwnd = glfwGetWin32Window(mWindowGLFW);

        mMenu.BeginHorizontalMenu(hwnd);

        mMenu.BeginVerticalMenu("File");
        mMenu.AddMenuItem("New", EMenuID::File_New);
        mMenu.AddMenuItem("Open", EMenuID::File_Open);
        mMenu.AddMenuItem("Create", EMenuID::File_Create);
        mMenu.AddSeparator();
        mMenu.AddMenuItem("Save", EMenuID::File_Save);
        mMenu.AddMenuItem("SaveAs", EMenuID::File_SaveAs);
        mMenu.AddSeparator();
        mMenu.AddMenuItem("Perference", EMenuID::File_EditPerference);
        mMenu.AddSeparator();
        mMenu.AddMenuItem("Exit", EMenuID::File_Exit);
        mMenu.EndVerticalMenu();

        mMenu.BeginVerticalMenu("View");
        mMenu.AddMenuItem("Wireframe", EMenuID::View_Wireframe);
        mMenu.AddMenuItem("Solid", EMenuID::View_Solid);
        mMenu.AddMenuItem("Debug", EMenuID::View_Debug);
        mMenu.AddSeparator();

        mMenu.BeginVerticalMenu("Select");
        mMenu.AddMenuItem("Point", EMenuID::View_Select_Point);
        mMenu.AddMenuItem("Line", EMenuID::View_Select_Line);
        mMenu.AddMenuItem("Face", EMenuID::View_Select_Face);
        mMenu.AddMenuItem("Mesh", EMenuID::View_Select_Object);
        mMenu.AddMenuItem("Object", EMenuID::View_Select_Object);
        mMenu.AddMenuItem("By Box", EMenuID::View_Select_ByBox);
        mMenu.EndVerticalMenu();

        mMenu.EndVerticalMenu();

        mMenu.BeginVerticalMenu("Edit");
        mMenu.AddMenuItem("Copy", EMenuID::Edit_Copy);
        mMenu.AddMenuItem("Cut", EMenuID::Edit_Cut);
        mMenu.AddMenuItem("Paste", EMenuID::Edit_Paste);
        mMenu.AddSeparator();
        
        mMenu.BeginVerticalMenu("Add Primitive");
        mMenu.AddMenuItem("Point", EMenuID::Edit_AddPrimitive_Point);
        mMenu.AddMenuItem("Line", EMenuID::Edit_AddPrimitive_Line);
        mMenu.AddMenuItem("Plane", EMenuID::Edit_AddPrimitive_Plane);
        mMenu.AddMenuItem("Cube", EMenuID::Edit_AddPrimitive_Cube);
        mMenu.AddMenuItem("Sphere", EMenuID::Edit_AddPrimitive_Sphere);
        mMenu.AddMenuItem("Torus", EMenuID::Edit_AddPrimitive_Torus);
        mMenu.EndVerticalMenu();

        mMenu.BeginVerticalMenu("Add Model");
        mMenu.AddMenuItem("Bunny", EMenuID::Edit_AddModel_Bunny);
        mMenu.AddMenuItem("Dragon", EMenuID::Edit_AddModel_Dragon);
        mMenu.EndVerticalMenu();

        mMenu.EndVerticalMenu();

        mMenu.BeginVerticalMenu("Tools");
        mMenu.EndVerticalMenu();

        mMenu.EndHorizontalMenu();
        
        local::glfwProc = (WNDPROC)GetWindowLongPtrW(hwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)local::windowProcNew);
    }
}
#endif