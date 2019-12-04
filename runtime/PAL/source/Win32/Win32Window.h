#pragma once

#include "Wanlix/PAL/Window.h"
#include <Windows.h>

namespace Wanlix
{
    class Win32Window final : public Window
    {
        friend class Win32WindowClass;
    public:
        using Ptr       = std::shared_ptr<Win32Window>;
        using ConstPtr  = std::shared_ptr<const Win32Window>;
        using UniquePtr = std::unique_ptr<Win32Window>;

        static UniquePtr Create(const Window::Desc& desc);
        static UniquePtr Attach(HWND handle);
        ~Win32Window();

        void SetPosition(const INT2& pos) override;
        void SetSize(const INT2& size, bool clientArea) override;
        void SetTitle(const std::wstring& title) override;
        void Show() override;
        void Hide() override;
        void SetDescriptor(const Window::Desc& desc) override;

        void*        GetHandle() const override;
        INT2         GetContentSize() const override;
        INT2         GetSize(bool clientArea) const override;
        INT2         GetPosition() const override;
        std::wstring GetTitle() const override;
        bool         IsVisible() const override;
        Window::Desc GetDescriptor() const override;
        bool         ProcessEvents() const override;
        
        void PostEvent(WindowEvent event, long arg1 = 0, long arg2 = 0);

    private:
        Win32Window(const Window::Desc& desc);
        HWND CreateWindowHandle(Window::Desc desc);

    private:
        HWND mHwnd = nullptr;
        HWND mParent = nullptr;
    };
}