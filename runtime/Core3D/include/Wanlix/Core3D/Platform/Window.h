#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Wanlix/Core3D/IModule.h"
#include "Wanlix/Core3D/RHI/Surface.h"
#include "Wanlix/Core3D/Utility/Signal.h"
#include "WindowDescriptor.h"
#include "Display.h"
#include "Key.h"

namespace Wanlix
{
    class Window : public Surface
    {
    public:
        using Ptr = std::shared_ptr<Window>;
        using ConstPtr = std::shared_ptr<const Window>;
        using UniquePtr = std::unique_ptr<Window>;

        static UniquePtr Create(const WindowDescriptor& desc);
        static UniquePtr Attach(void* handle);

        virtual void SetPosition(const Offset& pos) = 0;
        virtual void SetSize(const Extent& size, bool clientArea) = 0;
        virtual void SetTitle(const std::wstring& title) = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Quit() = 0;
        virtual void SetDescriptor(const WindowDescriptor& desc) = 0;

        virtual Offset GetPosition() const = 0;
        virtual Extent GetSize(bool clientArea) const = 0;
        virtual std::wstring GetTitle() const = 0;
        virtual WindowDescriptor GetDescriptor() const = 0;
        virtual bool IsVisible() const = 0;

        bool Tick();
        SurfaceType GetSurfaceType() const override final;
        bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) override final;
        Display::UniquePtr GetResidentDisplay() const override final;

    public:
        Signal<void(Window&)> OnDraw;
        Signal<void(Window&)> OnQuit;
        Signal<void(Window&, Key)> OnKeyDown;
        Signal<void(Window&, Key)> OnKeyUp;
        Signal<void(Window&, Key)> OnDoubleClick;
        Signal<void(Window&, wchar_t)> OnChar;
        Signal<void(Window&, int)> OnWheelMotion;
        Signal<void(Window&, const Offset&)> OnMouseMotion;
        Signal<void(Window&, const Extent&)> OnResize;
        
    protected:
        Window();
        virtual void ProcessEventsInternal() = 0;

    protected:
        bool mAttached = false;
        bool mQuitFlag = false;
    };
}