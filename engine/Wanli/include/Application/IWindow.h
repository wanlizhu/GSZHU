#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Application/IMonitor.h"
#include "Application/IEventListener.h"

namespace Wanli
{
    struct WindowCreateInfo
    {
        const char* title = "Wanli Engine";
        glm::vec2 position = { 0, 0 };
        glm::vec2 size = { 0, 0 };
        bool fullscreen = false;
        bool visible = true;
        bool borderless = false;
        bool resizable = true;
        bool acceptDropFiles = true;
        Uint flags = 0;
    };

    class IWindow
    {
    public:
        virtual ~IWindow() = default;
        virtual bool Initialize(const WindowCreateInfo& info) = 0;
        virtual void Tick(double time, double delta) = 0;
        virtual void Destroy() = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void MoveTo(int x, int y) = 0;
        virtual bool Contains(const glm::vec2& pos) const = 0;
        virtual bool IsFullscreen() const = 0;
        virtual bool IsVisible() const = 0;
        virtual bool IsForeground() const = 0;
        virtual bool IsResizable() const = 0;
        virtual bool IsFocused() const = 0;
        virtual void SetTitle(const char* title) = 0;
        virtual void SetSize(int width, int height) = 0;
        virtual void SetForeground(bool force = false) = 0;
        virtual void SetResizable(bool enabled) = 0;
        virtual void SetFocused() = 0;
        virtual void SetOpacity(float opacity) = 0;
        virtual void SetDPIScale(float factor) = 0;
        virtual void SetEventListener(IEventListener* listener) = 0;
        virtual const char* GetTitle() const = 0;
        virtual void* GetHandle() const = 0;
        virtual float GetDPIScale() const = 0;
        virtual glm::vec2 GetPosition() const = 0;
        virtual glm::vec2 GetSize() const = 0;
        virtual IMonitor* GetMonitor() const = 0;
        virtual IEventListener* GetEventListener() const = 0;
    };
}