#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Wanlix/Core3D/Graphics/RHI/Surface.h"
#include "Wanlix/Core3D/Utility/Signal.h"
#include "Display.h"
#include "MapKey.h"

namespace Wanlix
{
    struct WindowDescriptor
    {
        std::wstring title = L"Untitled";
        Offset position = { 0, 0 };
        Extent size = { 0, 0 };
        bool   visible = true;
        bool   borderless = false;
        bool   resizable = true;
        bool   acceptDropFiles = true;
        bool   centered = true;
        void*  parent = nullptr;
    };

    class Window : public Surface
    {
    public:
        using Ptr       = std::shared_ptr<Window>;
        using ConstPtr  = std::shared_ptr<const Window>;
        using UniquePtr = std::unique_ptr<Window>;

        static UniquePtr Create(const WindowDescriptor& desc);
        static UniquePtr Attach(void* handle);

        virtual void SetPosition(const Offset& pos) = 0;
        virtual void SetSize(const Extent& size, bool clientArea) = 0;
        virtual void SetTitle(WStringCRef title) = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Quit() = 0;
        virtual void SetDescriptor(const WindowDescriptor& desc) = 0;

        virtual Offset           GetPosition() const = 0;
        virtual Extent           GetSize(bool clientArea) const = 0;
        virtual std::wstring     GetTitle() const = 0;
        virtual WindowDescriptor GetDescriptor() const = 0;
        virtual bool IsVisible() const = 0;

        bool Tick();
        SurfaceType GetSurfaceType() const override final;
        bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) override final;
        Display::UniquePtr GetResidentDisplay() const;

    public:
        Signal<void(Window&)>                OnDraw;
        Signal<void(Window&)>                OnQuit;
        Signal<void(Window&, Key)>           OnKeyDown;
        Signal<void(Window&, Key)>           OnKeyUp;
        Signal<void(Window&, Key)>           OnDoubleClick;
        Signal<void(Window&, wchar_t)>       OnChar;
        Signal<void(Window&, int)>           OnWheelMotion;
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