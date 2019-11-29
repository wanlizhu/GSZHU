#pragma once

#include <Cocoa/Cocoa.h>
#include "Wanlix/Core3D/Platform/Window.h"

namespace Wanlix
{
    class MacOSWindow : public Window
    {
    public:
        using Ptr = std::shared_ptr<MacOSWindow>;
        using ConstPtr = std::shared_ptr<const MacOSWindow>;
        using UniquePtr = std::unique_ptr<MacOSWindow>;

        static UniquePtr Create(const WindowDescriptor& desc);
        ~MacOSWindow();

        void SetPosition(const Offset& pos) override;
        void SetSize(const Extent& size, bool clientArea) override;
        void SetTitle(WStringCRef title) override;
        void Show() override;
        void Hide() override;
        void Quit() override;
        void SetDescriptor(const WindowDescriptor& desc) override;

        void* GetNativeHandle() const override;
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