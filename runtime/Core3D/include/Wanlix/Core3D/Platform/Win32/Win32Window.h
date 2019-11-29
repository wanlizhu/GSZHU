#pragma once

#include "Wanlix/Core3D/Platform/Window.h"
#include "Win32LeanAndMean.h"
#include <Windows.h>

namespace Wanlix
{
    class Win32Window final : public Window
    {
    public:
        using Ptr       = std::shared_ptr<Win32Window>;
        using ConstPtr  = std::shared_ptr<const Win32Window>;
        using UniquePtr = std::unique_ptr<Win32Window>;

        static UniquePtr Create(const WindowDescriptor& desc);
        static UniquePtr Attach(HWND handle);
        ~Win32Window();

        void SetPosition(const Offset& pos) override;
        void SetSize(const Extent& size, bool clientArea) override;
        void SetTitle(WStringCRef title) override;
        void Show() override;
        void Hide() override;
        void Quit() override;
        void SetDescriptor(const WindowDescriptor& desc) override;

        void*    GetNativeHandle() const override;
        Extent   GetContentSize() const override;
        Extent   GetSize(bool clientArea) const override;
        Offset   GetPosition() const override;
        WString  GetTitle() const override;
        bool     IsVisible() const override;
        uint32_t GetMoveAndResizeTimerId() const;
        WindowDescriptor GetDescriptor() const override;
        
    private:
        Win32Window(const WindowDescriptor& desc);
        Win32Window(HWND handle);
        void ProcessEventsInternal() override;
        HWND CreateWindowHandle(WindowDescriptor desc);
        void AttachHandle(HWND handle);
        void DetachHandle(HWND handle);

    private:
        HWND mHwnd = nullptr;
        HWND mParent = nullptr;

        // Set a timer during a window is moved or resized to make continous scene updates.
        uint32_t mMoveAndResizeTimerId = 1;
        WNDPROC  mPreviousWindowProc = nullptr;
    };
}