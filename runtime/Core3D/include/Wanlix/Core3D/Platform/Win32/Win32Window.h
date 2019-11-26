#pragma once

#include "Wanlix/Core3D/Platform/IWindow.h"
#include "Win32LeanAndMean.h"
#include <Windows.h>

namespace Wanlix
{
    class Win32Window final : public IWindow
    {
    public:
        using Ptr = std::shared_ptr<Win32Window>;
        using ConstPtr = std::shared_ptr<const Win32Window>;
        using UniquePtr = std::unique_ptr<Win32Window>;

        static UniquePtr Create(const WindowDescriptor& desc);
        ~Win32Window();

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
        uint32_t GetMoveAndResizeTimerId() const;

    private:
        Win32Window(const WindowDescriptor& desc);
        void ProcessEventsInternal() override;
        HWND CreateWindowHandle(WindowDescriptor desc);

    private:
        HWND mHwnd = nullptr;
        HWND mParent = nullptr;
        // Set a timer during a window is moved or resized to make continous scene updates.
        uint32_t mMoveAndResizeTimerId = 1;
    };
}