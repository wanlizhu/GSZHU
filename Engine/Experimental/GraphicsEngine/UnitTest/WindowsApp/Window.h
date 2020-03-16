#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <functional>

class Window
{
public:
    using PFN_Callback = std::function<void(WPARAM, LPARAM)>;
    using PFN_WndProc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

    Window();
    virtual ~Window();

    bool Create(PCWSTR title, int w, int h);
    void PushWndProc(const PFN_WndProc& wndProc);
    void PopWndProc();
    HWND GetHwnd() const;
    void SetCallback(UINT msg, const PFN_Callback& callback);

private:
    static void RegisterWindowClass();
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
    static PCWSTR smWindowClassName;
    HWND mHwnd = NULL;
    std::unordered_map<UINT, PFN_Callback> mCallbacks;
    std::stack<PFN_WndProc> mWndProcStack;
};