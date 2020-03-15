#include "AcadDM-Vulkan-App-Window.h"
#include <assert.h>

PCWSTR Window::smWindowClassName = L"AacdDM-Vulkan-App";

Window::Window()
{
    static bool _inited = false;
    if (!_inited)
    {
        _inited = true;
        RegisterWindowClass();
    }
}

Window::~Window()
{
    ::DestroyWindow(mHwnd);
    mHwnd = NULL;
}

bool Window::Create(PCWSTR title, int w, int h)
{
    mHwnd = ::CreateWindowEx(
        NULL,
        smWindowClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        w, h,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    if (mHwnd)
    {
        ::SetWindowLongPtr(mHwnd, GWLP_USERDATA, (LONG_PTR)this);
        PushWndProc(&Window::WndProc);
        ::ShowWindow(mHwnd, SW_SHOW);
        ::UpdateWindow(mHwnd);
    }

    return mHwnd != NULL;
}

void Window::PushWndProc(const PFN_WndProc& wndProc)
{
    mWndProcStack.push(wndProc);
    ::SetWindowLongPtr(mHwnd, GWLP_WNDPROC, (LONG_PTR)mWndProcStack.top().target<LRESULT(HWND, UINT, WPARAM, LPARAM)>());
}

void Window::PopWndProc()
{
    if (mWndProcStack.empty())
        return;

    mWndProcStack.pop();
    ::SetWindowLongPtr(mHwnd, GWLP_WNDPROC, (LONG_PTR)mWndProcStack.top().target<LRESULT(HWND, UINT, WPARAM, LPARAM)>());
}

HWND Window::GetHwnd() const
{
    return mHwnd;
}

void Window::SetCallback(UINT msg, const PFN_Callback& callback)
{
    mCallbacks[msg] = callback;
}

void Window::RegisterWindowClass()
{
    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &Window::WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = smWindowClassName;
    wc.hIconSm = NULL;

    ATOM result = ::RegisterClassExW(&wc);
    assert(result != NULL);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (window)
    {
        const auto& it = window->mCallbacks.find(message);
        if (it != window->mCallbacks.end() && it->second)
        {
            it->second(wParam, lParam);
        }
    }

    switch (message)
    {
    case WM_CREATE:
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(hwnd, &ps);
        ::EndPaint(hwnd, &ps);
    }
    break;

    case WM_COMMAND:
    {
        INT menuId = LOWORD(wParam);
    }
    break;
    }

    return ::DefWindowProc(hwnd, message, wParam, lParam);
}
