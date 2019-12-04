#include "Win32Window.h"
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
        INT2  position;
        INT2  size;
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

        if (desc.windowContext != nullptr)
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

    static INT2 GetScreenCenteredPosition(const INT2& size)
    {
        return INT2(::GetSystemMetrics(SM_CXSCREEN) / 2 - (size.width / 2),
                    ::GetSystemMetrics(SM_CYSCREEN) / 2 - (size.height / 2));
    }

    static WindowAppearance GetWindowAppearance(const WindowDescriptor& desc)
    {
        WindowAppearance appearance;

        // Get window style and client area 
        appearance.style = GetWindowStyle(desc);

        auto rc = GetClientArea(
            (LONG)(desc.size.width),
            (LONG)(desc.size.height),
            appearance.style
        );

        // Setup window size 
        appearance.size.width = (uint32_t)(rc.right - rc.left);
        appearance.size.height = (uint32_t)(rc.bottom - rc.top);

        // Setup window position 
        appearance.position = (desc.centered ? GetScreenCenteredPosition(desc.size) : desc.position);

        if (desc.centered)
        {
            appearance.position.x += rc.left;
            appearance.position.y += rc.top;
        }

        return appearance;
    }

    Window::UniquePtr Window::Create(const Window::Desc& desc)
    {
        return Win32Window::Create(desc);
    }

    Win32Window::UniquePtr Win32Window::Create(const Window::Desc& desc)
    {
        return std::unique_ptr<Win32Window>(new Win32Window(desc));
    }

    Win32Window::Win32Window(const Window::Desc& desc)
        : Window()
        , mHwnd(CreateWindowHandle(desc))
    {}

    Win32Window::~Win32Window()
    {
        if (mHwnd) {
            ::DestroyWindow(mHwnd);
            mHwnd = nullptr;
        }
    }

    void* Win32Window::GetHandle() const
    {
        return (void*)mHwnd;
    }

    void Win32Window::SetPosition(const INT2& pos)
    {
        ::SetWindowPos(mHwnd, HWND_TOP, pos.x, pos.y, 0, 0, (SWP_NOSIZE | SWP_NOZORDER));
    }

    INT2 Win32Window::GetPosition() const
    {
        RECT rc;
        ::GetWindowRect(mHwnd, &rc);
        ::MapWindowPoints(HWND_DESKTOP, GetParent(mHwnd), reinterpret_cast<LPPOINT>(&rc), 2);
        return INT2(rc.left, rc.top);
    }

    void Win32Window::SetSize(const INT2& size, bool clientArea)
    {
        int cx, cy;

        if (clientArea)
        {
            auto rc = GetClientArea(
                (LONG)(size.width),
                (LONG)(size.height),
                ::GetWindowLongW(mHwnd, GWL_STYLE)
            );
            cx = rc.right - rc.left;
            cy = rc.bottom - rc.top;
        }
        else
        {
            cx = (int)(size.width);
            cy = (int)(size.height);
        }

        ::SetWindowPos(mHwnd, HWND_TOP, 0, 0, cx, cy, (SWP_NOMOVE | SWP_NOZORDER));
    }

    INT2 Win32Window::GetContentSize() const
    {
        return GetSize(true);
    }

    INT2 Win32Window::GetSize(bool clientArea) const
    {
        if (clientArea)
        {
            RECT rc;
            ::GetClientRect(mHwnd, &rc);
            return
            {
                (int)(rc.right - rc.left),
                (int)(rc.bottom - rc.top)
            };
        }
        else
        {
            RECT rc;
            ::GetWindowRect(mHwnd, &rc);
            return
            {
                (int)(rc.right - rc.left),
                (int)(rc.bottom - rc.top)
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
        desc.windowContext = mParent;

        return desc;
    }

    void Win32Window::SetDescriptor(const Window::Desc& desc)
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
                (int)(appearance.size.width),
                (int)(appearance.size.height),
                flags
            );

            // Reset user data
            SetUserData(mHwnd, this);
        }
    }

    bool Win32Window::ProcessEvents() const
    {
        // Peek all queued messages 
        MSG message;

        while (::PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessageW(&message);
        }

        return !ShouldQuit();
    }

    HWND Win32Window::CreateWindowHandle(Window::Desc desc)
    {
        auto windowClass = Win32WindowClass::Instance();

        // Populate default fields
        if (desc.size <= 0) {
            desc.size.width = (int)(::GetSystemMetrics(SM_CXSCREEN) / 1.414);
            desc.size.height = (int)(::GetSystemMetrics(SM_CYSCREEN) / 1.414);
        }

        // Get final window size 
        auto appearance = GetWindowAppearance(desc);

        // Get parent window
        HWND parentWnd = HWND_DESKTOP;

        if (auto parent = reinterpret_cast<HWND>(const_cast<void*>(desc.windowContext))) {
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
            (int)(appearance.size.width),
            (int)(appearance.size.height),
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

    void Win32Window::PostEvent(WindowEvent event, long arg1, long arg2)
    {
        switch (event) 
        {
        case WindowEvent::Quit: {
            for (auto& listener : mListeners) {
                listener->OnQuit(*this);
            }
            break;
        }
        case WindowEvent::Draw: {
            for (auto& listener : mListeners) {
                listener->OnDraw(*this);
            }
            break;
        }
        case WindowEvent::GetFocus: {
            for (auto& listener : mListeners) {
                listener->OnGetFocus(*this);
            }
            break;
        }
        case WindowEvent::LostFocus: {
            for (auto& listener : mListeners) {
                listener->OnLostFocus(*this);
            }
            break;
        }
        case WindowEvent::KeyDown: {
            for (auto& listener : mListeners) {
                listener->OnKeyDown(*this, static_cast<Key>(arg1));
            }
            break;
        }
        case WindowEvent::KeyUp: {
            for (auto& listener : mListeners) {
                listener->OnKeyUp(*this, static_cast<Key>(arg1));
            }
            break;
        }
        case WindowEvent::DoubleClick: {
            for (auto& listener : mListeners) {
                listener->OnDoubleClick(*this, static_cast<Key>(arg1));
            }
            break;
        }
        case WindowEvent::Char: {
            for (auto& listener : mListeners) {
                listener->OnChar(*this, static_cast<wchar_t>(arg1));
            }
            break;
        }
        case WindowEvent::WheelMotion: {
            for (auto& listener : mListeners) {
                listener->OnWheelMotion(*this, arg1);
            }
            break;
        }
        case WindowEvent::LocalMotion: {
            for (auto& listener : mListeners) {
                listener->OnLocalMotion(*this, INT2(arg1, arg2));
            }
            break;
        }
        case WindowEvent::GlobalMotion: {
            for (auto& listener : mListeners) {
                listener->OnGlobalMotion(*this, INT2(arg1, arg2));
            }
            break;
        }
        case WindowEvent::Resize: {
            for (auto& listener : mListeners) {
                listener->OnResize(*this, INT2(arg1, arg2));
            }
            break;
        }
        default:
            break;
        }
    }
}