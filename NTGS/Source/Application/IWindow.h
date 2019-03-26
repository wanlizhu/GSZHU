#pragma once

#include <string>
#include "Application/WindowConfig.h"
#include "Common/Signal.h"
#include <any>
#include <memory>

namespace NTGS {
    class IWindow;
    typedef std::shared_ptr<IWindow> IWindowPtr;

    class IWindow : public std::enable_shared_from_this<IWindow> {
        friend class IApplication;
    public:
        using CloseEvent = Signal<void(const IWindow&)>;
        using ActiveEvent = Signal<void(const IWindow&, bool active)>;
        using PaintEvent = Signal<void(const IWindow&)>;
        using SizeEvent = Signal<void(const IWindow&, bool active)>;
        using KeyUpEvent = Signal<void(const IWindow&, int key, int mods)>;
        using KeyDownEvent = Signal<void(const IWindow&, int key, int mods)>;
        using MouseUpEvent = Signal<void(const IWindow&, int key)>;
        using MouseDownEvent = Signal<void(const IWindow&, int key)>;
        using MouseWheelEvent = Signal<void(const IWindow&, int delta)>;
        using MouseMoveEvent = Signal<void(const IWindow&, const Vec2i& pos)>;

    public:
        CloseEvent mCloseEvent;
        ActiveEvent mActiveEvent;
        PaintEvent mPaintEvent;
        SizeEvent mSizeEvent;
        KeyUpEvent mKeyUpEvent;
        KeyDownEvent mKeyDownEvent;
        MouseUpEvent mMouseUpEvent;
        MouseDownEvent mMouseDownEvent;
        MouseWheelEvent mMouseWheelEvent;
        MouseMoveEvent mMouseMoveEvent;

    public:
        virtual ~IWindow() {}
        virtual void Destroy() = 0;
        virtual void KeepScreenOn() = 0;
        virtual void* GetID() const = 0;
        virtual const std::string& GetName() const = 0;
        virtual const Vec2i& GetPosition() const = 0;
        virtual const Vec2i& GetSize() const = 0;

    protected:
        IWindow() = default;
        IWindow(const IWindow&) = default;
        IWindow& operator=(const IWindow&) = default;
    };
}