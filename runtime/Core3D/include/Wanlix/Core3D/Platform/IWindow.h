#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Wanlix/Core3D/IModule.h"
#include "Wanlix/Core3D/RHI/ISurface.h"
#include "Wanlix/Core3D/Utility/Signal.h"
#include "WindowDescriptor.h"
#include "IDisplay.h"
#include "Key.h"

namespace Wanlix
{
    class IWindow : public ISurface, public IModule
    {
    public:
        using Ptr = std::shared_ptr<IWindow>;
        using ConstPtr = std::shared_ptr<const IWindow>;
        using UniquePtr = std::unique_ptr<IWindow>;

        static UniquePtr Create(const WindowDescriptor& desc);

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

        bool Tick(float delta) override final;
        bool ProcessEvents() override final;
        bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) override final;
        IDisplay::UniquePtr GetResidentDisplay() const override final;

    public:
        Signal<void(IWindow& sender)> OnProcessEvents;
        Signal<void(IWindow& sender)> OnDraw;
        Signal<void(IWindow& sender)> OnQuit;
        Signal<void(IWindow& sender)> OnGetFocus;
        Signal<void(IWindow& sender)> OnLostFocus;
        Signal<void(IWindow& sender, Key key)> OnKeyDown;
        Signal<void(IWindow& sender, Key key)> OnKeyUp;
        Signal<void(IWindow& sender, Key btn)> OnDoubleClick;
        Signal<void(IWindow& sender, wchar_t chr)> OnChar;
        Signal<void(IWindow& sender, int motion)> OnWheelMotion;
        Signal<void(IWindow& sender, const Offset& pos)> OnMouseMotion;
        Signal<void(IWindow& sender, const Extent& size)> OnResize;
        
    protected:
        IWindow(std::weak_ptr<Context> context);
        virtual void ProcessEventsInternal() = 0;

    protected:
        bool mQuitFlag = false;
    };
}