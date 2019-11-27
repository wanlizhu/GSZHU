#include "Wanlix/Core3D/Platform/Win32/Win32Window.h"
#include "Win32WindowClass.h"
#include <string>
#include <exception>
#include <stdexcept>
#include <shellapi.h>
#include <WinUser.h>

namespace Wanlix
{
    struct WindowAppearance
    {
        DWORD style = 0;
        Offset position;
        Extent size;
    };

    static void SetUserData(HWND hwnd, void* data)
    {
        ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
    }

    static RECT GetClientArea(LONG width, LONG height, DWORD style)
    {
        RECT rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = width;
        rect.bottom = height;
        ::AdjustWindowRect(&rect, style, FALSE);
        return rect;
    }

    // Determines the Win32 window style for the specified descriptor.
    static DWORD GetWindowStyle(const WindowDescriptor& desc)
    {
        DWORD style = (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

        if (desc.parent != nullptr)
            style |= WS_CHILD;
        else if (desc.borderless)
            style |= WS_POPUP;
        else
        {
            style |= (WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION);
            if (desc.resizable)
                style |= (WS_SIZEBOX | WS_MAXIMIZEBOX);
        }

        if (desc.visible)
            style |= WS_VISIBLE;

        if (desc.acceptDropFiles)
            style |= WM_DROPFILES;

        return style;
    }

    static Offset GetScreenCenteredPosition(const Extent& size)
    {
        return
        {
            ::GetSystemMetrics(SM_CXSCREEN) / 2 - static_cast<int>(size.width / 2),
            ::GetSystemMetrics(SM_CYSCREEN) / 2 - static_cast<int>(size.height / 2)
        };
    }

    static WindowAppearance GetWindowAppearance(const WindowDescriptor& desc)
    {
        WindowAppearance appearance;

        // Get window style and client area 
        appearance.style = GetWindowStyle(desc);

        auto rc = GetClientArea(
            static_cast<LONG>(desc.size.width),
            static_cast<LONG>(desc.size.height),
            appearance.style
        );

        // Setup window size 
        appearance.size.width = static_cast<uint32_t>(rc.right - rc.left);
        appearance.size.height = static_cast<uint32_t>(rc.bottom - rc.top);

        // Setup window position 
        appearance.position = (desc.centered ? GetScreenCenteredPosition(desc.size) : desc.position);

        if (desc.centered)
        {
            appearance.position.x += rc.left;
            appearance.position.y += rc.top;
        }

        return appearance;
    }

    IWindow::UniquePtr IWindow::Create(const WindowDescriptor& desc)
    {
        return Win32Window::Create(desc);
    }

    IWindow::UniquePtr IWindow::Attach(void* handle)
    {
        return Win32Window::Attach((HWND)handle);
    }

    Win32Window::UniquePtr Win32Window::Create(const WindowDescriptor& desc)
    {
        return std::unique_ptr<Win32Window>(new Win32Window(desc));
    }

    Win32Window::UniquePtr Win32Window::Attach(HWND handle)
    {
        return std::unique_ptr<Win32Window>(new Win32Window(handle));
    }

    Win32Window::Win32Window(const WindowDescriptor& desc)
        : IWindow()
        , mHwnd(CreateWindowHandle(desc))
    {}

    Win32Window::Win32Window(HWND handle)
        : IWindow()
        , mHwnd(handle)
    {
        mAttached = true;
        AttachHandle(mHwnd);
    }

    Win32Window::~Win32Window()
    {
        if (mAttached) {
            DetachHandle(mHwnd);
            mHwnd = nullptr;
            return;
        }

        if (mHwnd) {
            ::DestroyWindow(mHwnd);
            mHwnd = nullptr;
        }
    }

    void* Win32Window::GetNativeHandle() const
    {
        return (void*)mHwnd;
    }

    void Win32Window::SetPosition(const Offset& pos)
    {
        ::SetWindowPos(mHwnd, HWND_TOP, pos.x, pos.y, 0, 0, (SWP_NOSIZE | SWP_NOZORDER));
    }

    Offset Win32Window::GetPosition() const
    {
        RECT rc;
        ::GetWindowRect(mHwnd, &rc);
        ::MapWindowPoints(HWND_DESKTOP, GetParent(mHwnd), reinterpret_cast<LPPOINT>(&rc), 2);
        return { rc.left, rc.top };
    }

    void Win32Window::SetSize(const Extent& size, bool clientArea)
    {
        int cx, cy;

        if (clientArea)
        {
            auto rc = GetClientArea(
                static_cast<LONG>(size.width),
                static_cast<LONG>(size.height),
                ::GetWindowLongW(mHwnd, GWL_STYLE)
            );
            cx = rc.right - rc.left;
            cy = rc.bottom - rc.top;
        }
        else
        {
            cx = static_cast<int>(size.width);
            cy = static_cast<int>(size.height);
        }

        ::SetWindowPos(mHwnd, HWND_TOP, 0, 0, cx, cy, (SWP_NOMOVE | SWP_NOZORDER));
    }

    Extent Win32Window::GetContentSize() const
    {
        return GetSize(true);
    }

    Extent Win32Window::GetSize(bool clientArea) const
    {
        if (clientArea)
        {
            RECT rc;
            ::GetClientRect(mHwnd, &rc);
            return
            {
                static_cast<uint32_t>(rc.right - rc.left),
                static_cast<uint32_t>(rc.bottom - rc.top)
            };
        }
        else
        {
            RECT rc;
            ::GetWindowRect(mHwnd, &rc);
            return
            {
                static_cast<uint32_t>(rc.right - rc.left),
                static_cast<uint32_t>(rc.bottom - rc.top)
            };
        }
    }

    void Win32Window::SetTitle(const std::wstring& title)
    {
        ::SetWindowTextW(mHwnd, title.c_str());
    }

    std::wstring Win32Window::GetTitle() const
    {
        wchar_t title[MAX_PATH];
        ::GetWindowTextW(mHwnd, title, MAX_PATH);
        return std::wstring(title);
    }

    void Win32Window::Show()
    {
        ::ShowWindow(mHwnd,  SW_NORMAL);
    }

    void Win32Window::Hide()
    {
        ::ShowWindow(mHwnd, SW_HIDE);
    }

    void Win32Window::Quit()
    {
        mQuitFlag = true;
        OnQuit(*this);
    }

    bool Win32Window::IsVisible() const
    {
        return IsWindowVisible(mHwnd) == TRUE;
    }

    WindowDescriptor Win32Window::GetDescriptor() const
    {
        // Get window flags and other information for comparision
        auto windowFlags = ::GetWindowLongW(mHwnd, GWL_STYLE);
        auto windowSize = GetContentSize();
        auto centerPoint = GetScreenCenteredPosition(windowSize);

        // Setup window descriptor 
        WindowDescriptor desc;

        desc.title = GetTitle();
        desc.position = GetPosition();
        desc.size = windowSize;

        desc.visible = ((windowFlags & WS_VISIBLE) != 0);
        desc.borderless = ((windowFlags & WS_CAPTION) == 0);
        desc.resizable = ((windowFlags & WS_SIZEBOX) != 0);
        desc.acceptDropFiles = ((windowFlags & WM_DROPFILES) != 0);
        desc.centered = (centerPoint.x == desc.position.x && centerPoint.y == desc.position.y);
        desc.parent = mParent;

        return desc;
    }

    void Win32Window::SetDescriptor(const WindowDescriptor& desc)
    {
        // Get current window flags
        auto windowFlags = GetWindowLong(mHwnd, GWL_STYLE);

        auto borderless = ((windowFlags & WS_CAPTION) == 0);
        auto resizable = ((windowFlags & WS_SIZEBOX) != 0);

        // Setup new window flags
        auto newWindowFlags = GetWindowStyle(desc);

        if ((windowFlags & WS_MAXIMIZE) != 0)
            newWindowFlags |= WS_MAXIMIZE;
        if ((windowFlags & WS_MINIMIZE) != 0)
            newWindowFlags |= WS_MINIMIZE;

        auto flagsChanged = (windowFlags != newWindowFlags);

        // Check if anything changed
        auto position = GetPosition();
        auto size = GetContentSize();

        bool positionChanged = (desc.position.x != position.x || desc.position.y != position.y);
        bool sizeChanged = (desc.size.width != size.width || desc.size.height != size.height);

        if (flagsChanged || positionChanged || sizeChanged)
        {
            // Temporary reset user data to avoid recursive resize events
            SetUserData(mHwnd, nullptr);

            UINT flags = SWP_NOZORDER;

            if (flagsChanged)
            {
                // Hide temporarily to avoid strange effects during frame change (if frame has changed) 
                ::ShowWindow(mHwnd, SW_HIDE);

                // Set new window style
                ::SetWindowLongPtrW(mHwnd, GWL_STYLE, newWindowFlags);
                flags |= SWP_FRAMECHANGED;
            }

            // Set new position and size
            auto appearance = GetWindowAppearance(desc);

            if (desc.visible)
                flags |= SWP_SHOWWINDOW;

            if ((newWindowFlags & WS_MAXIMIZE) != 0)
                flags |= (SWP_NOSIZE | SWP_NOMOVE);

            if (borderless == desc.borderless && resizable == desc.resizable)
            {
                if (!positionChanged)
                    flags |= SWP_NOMOVE;
                if (!sizeChanged)
                    flags |= SWP_NOSIZE;
            }

            SetWindowPos(
                mHwnd,
                0, // ignore, due to SWP_NOZORDER flag
                appearance.position.x,
                appearance.position.y,
                static_cast<int>(appearance.size.width),
                static_cast<int>(appearance.size.height),
                flags
            );

            // Reset user data
            SetUserData(mHwnd, this);
        }
    }

    void Win32Window::ProcessEventsInternal()
    {
        // Peek all queued messages 
        MSG message;

        while (::PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessageW(&message);
        }
    }

    HWND Win32Window::CreateWindowHandle(WindowDescriptor desc)
    {
        auto windowClass = Win32WindowClass::Instance();

        // Populate default fields
        if (desc.size.IsEmpty()) {
            desc.size.width = (int)(::GetSystemMetrics(SM_CXSCREEN) / 1.414);
            desc.size.height = (int)(::GetSystemMetrics(SM_CYSCREEN) / 1.414);
        }

        // Get final window size 
        auto appearance = GetWindowAppearance(desc);

        // Get parent window
        HWND parentWnd = HWND_DESKTOP;

        if (auto parent = reinterpret_cast<HWND>(desc.parent)) {
            parentWnd = parent;
        }

        // Create frame window object
        HWND wnd = CreateWindowExW(
            NULL,
            windowClass->GetName(),
            desc.title.c_str(),
            appearance.style,
            appearance.position.x,
            appearance.position.y,
            static_cast<int>(appearance.size.width),
            static_cast<int>(appearance.size.height),
            parentWnd,
            nullptr,
            ::GetModuleHandleW(nullptr),
            nullptr
        );

        if (!wnd) {
            throw std::runtime_error("failed to create window");
        }

        #ifndef WANLIX_ARCH_ARM
        // Set additional flags
        if (desc.acceptDropFiles) {
            ::DragAcceptFiles(mHwnd, TRUE);
        }
        #endif

        // Set reference of this object to the window user-data 
        SetUserData(wnd, this);

        return wnd;
    }

    void Win32Window::AttachHandle(HWND handle)
    {
        mPreviousWindowProc = (WNDPROC)GetWindowLongPtrW(handle, GWLP_WNDPROC);
        SetWindowLongPtrW(handle, GWLP_WNDPROC, (LONG_PTR)&Win32WindowClass::WindowProc);
    }

    void Win32Window::DetachHandle(HWND handle)
    {
        if (!mAttached) {
            return;
        }

        if (mPreviousWindowProc) {
            SetWindowLongPtrW(handle, GWLP_WNDPROC, (LONG_PTR)mPreviousWindowProc);
            mPreviousWindowProc = nullptr;
        }
    }

    uint32_t Win32Window::GetMoveAndResizeTimerId() const
    {
        return mMoveAndResizeTimerId;
    }
}