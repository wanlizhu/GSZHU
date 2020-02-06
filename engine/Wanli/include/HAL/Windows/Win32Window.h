#pragma once

#include "Application/IWindow.h"
#include "HAL/Windows/Win32Monitor.h"
#include "HAL/Windows/Win32Common.h"


struct GLFWwindow;

namespace Wanli
{
    class Win32Window : public IWindow
    {
    public:
        Win32Window() = default;
        Win32Window(const Win32Window&) = delete;
        Win32Window& operator=(const Win32Window&) = delete;
        virtual ~Win32Window();

        virtual bool Initialize(const WindowCreateInfo& info) override final;
        virtual void Tick(double time, double delta) override final;
        virtual void Destroy() override final;
        virtual void Show() override final;
        virtual void Hide() override final;
        virtual void MoveTo(int x, int y) override final;
        virtual bool Contains(const glm::vec2& pos) const override final;
        virtual bool IsFullscreen() const override final;
        virtual bool IsVisible() const override final;
        virtual bool IsForeground() const override final;
        virtual bool IsResizable() const override final;
        virtual bool IsFocused() const override final;
        virtual void SetTitle(const char* title) override final;
        virtual void SetSize(int width, int height) override final;
        virtual void SetForeground(bool force = false) override final;
        virtual void SetResizable(bool enabled) override final;
        virtual void SetFocused() override final;
        virtual void SetOpacity(float opacity) override final;
        virtual void SetDPIScale(float factor) override final;
        virtual void SetEventListener(IEventListener* listener) override final;
        virtual const char* GetTitle() const override final;
        virtual void* GetHandle() const override final;
        virtual float GetDPIScale() const override final;
        virtual glm::vec2 GetPosition() const override final;
        virtual glm::vec2 GetSize() const override final;
        virtual Win32Monitor* GetMonitor() const override final;
        virtual IEventListener* GetEventListener() const override final;

    private:
        GLFWwindow* mGlfwHandle = nullptr;
        HWND mHWND = NULL;
        std::unique_ptr<Win32Monitor> mMonitor;
        IEventListener* mEventListener = nullptr;
    };
}