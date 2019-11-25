#pragma once

#include <Cocoa/Cocoa.h>
#include "Wanlix/Platform/IWindow.h"

namespace Wanlix
{
    class MacOSWindow : public IWindow
    {
    public:
        using Ptr = std::shared_ptr<Win32Window>;
        using ConstPtr = std::shared_ptr<const Win32Window>;
        using UniquePtr = std::unique_ptr<Win32Window>;

        static UniquePtr Create(const WindowDescriptor& desc);
        ~MacOSWindow();

        void ResetPixelFormat() override;
        void SetPosition(const Offset& pos) override;
        void SetSize(const Extent& size, bool clientArea) override;
        void SetTitle(const std::wstring& title) override;
        void Show() override;
        void Hide() override;
        void Quit() override;
        void SetDescriptor(const WindowDescriptor& desc) override;

        bool GetNativeHandle(void* handle, size_t handleSize) const override;
        Extent GetContentSize() const override;
        Extent GetSize(bool clientArea) const override;
        Offset GetPosition() const override;
        std::wstring GetTitle() const override;
        WindowDescriptor GetDescriptor() const override;
        bool IsVisible() const override;
        
        inline NSWindow* GetNSWindow() const { return mNsWindow; }

    private:
        void ProcessEventsInternal() override;
        void ProcessKeyEvent(NSEvent* event, bool down);
        void ProcessMouseKeyEvent(Key key, bool down);
        void ProcessMouseMoveEvent(NSEvent* event);
        void ProcessMouseWheelEvent(NSEvent* event);

        NSWindow* CreateNSWindow(const WindowDescriptor& desc);

    private:
        NSWindow* mNsWindow = nullptr;
        Offset mPrevMotionPos;
    };
}