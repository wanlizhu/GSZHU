#pragma once

#include "Wanlix/Core3D/Graphics/RHI/Surface.h"
#include "Wanlix/Core3D/Graphics/Types.h"
#include "MapKey.h"
#include "EventListener.h"
#include "Display.h"

namespace Wanlix
{
    struct CanvasDescriptor
    {
        std::wstring title;
        bool borderless = false;
    };

    class Canvas : public Surface
    {
    public:
        using Ptr       = std::shared_ptr<Canvas>;
        using ConstPtr  = std::shared_ptr<const Canvas>;
        using UniquePtr = std::unique_ptr<Canvas>;
        using Listener  = EventListener<SurfaceType::MobileCanvas>::Ptr;

        static UniquePtr Create(const CanvasDescriptor& desc);

        virtual void SetTitle(WStringCRef title) = 0;
        virtual WString GetTitle() const = 0;
        virtual bool ShouldQuit() const;

        bool Tick();
        SurfaceType GetSurfaceType() const override final;
        bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) override;
        Display::UniquePtr GetResidentDisplay() const;
        void AddEventListener(Listener listener);
        void RemoveEventListener(Listener listener);
        void PostEvent(Event event, std::any arg1 = 0, std::any arg2 = 0);

    protected:
        bool mQuitFlag = false;

    private:
        std::vector<Listener> mEventListeners;
    };
}