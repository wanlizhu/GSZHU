#pragma once

#include "Surface.h"
#include "MapKey.h"
#include "WindowEvent.h"

namespace Wanlix
{
    struct WindowDescriptor
    {
        std::wstring title;
        INT2 position;
        INT2 size;
        bool visible = false;
        bool centered = true;
        bool resizable = true;
        bool borderless = false;
        bool acceptDropFiles = true;
        const void* windowContext = nullptr;
    };

    class Window : public Surface
    {
    public:
        using SharedPtr = std::shared_ptr<Window>;
        using UniquePtr = std::unique_ptr<Window>;
        using Desc      = WindowDescriptor;
        using Listener  = std::shared_ptr<WindowEventListener>;

        static UniquePtr Create(const Desc& desc);

        virtual void SetPosition(const INT2& pos) = 0;
        virtual void SetSize(const INT2& size, bool useClientArea = true) = 0;
        virtual void SetTitle(const std::wstring& title) = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void SetDescriptor(const Desc& desc) = 0;

        virtual INT2 GetPosition() const = 0;
        virtual INT2 GetSize(bool useClientArea = true) const = 0;
        virtual std::wstring GetTitle() const = 0;
        virtual bool IsVisible() const = 0;
        virtual Desc GetDescriptor() const = 0;
        
        virtual bool AdaptForVideoMode(const VideoMode& videoMode) override final;
        virtual Display::UniquePtr GetResidentDisplay() const override final;

        void AddEventListener(const Listener& listener);
        void RemoveEventListener(const Listener& listener);
        bool HasFocus() const;
        bool ShouldQuit() const;

    protected:
        std::vector<Listener> mListeners;
        std::atomic_bool      mQuitFlag;
        std::atomic_bool      mFocused;
    };
}