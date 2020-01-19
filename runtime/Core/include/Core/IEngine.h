#pragma once

#include "Core/Common.h"
#include "Core/Window.h"
#include "RHI/IDevice.h"
#include "Utils/Clock.h"
#include "Utils/ArgList.h"

namespace Wanlix
{
    struct EngineDesc
    {
        WindowDesc windowDesc;
        DeviceDesc deviceDesc;
        bool showDialogOnError = true;
        float timeScale = 1.0f;
        bool pauseTime = false;
        bool showUi = true;
    };

    class IEngine
    {
    public:
        virtual IDrawContext* GetDrawContext() const = 0;
        virtual IFramebuffer* GetFramebuffer() const = 0;
        virtual Window* GetWindow() const = 0;
        virtual Clock GetClock() const = 0;
        virtual ArgList GetArgList() const = 0;
        virtual EngineDesc GetDesc() const = 0;
        virtual bool IsKeyDown(const EKey& key) const = 0;
        virtual void ShowUi(bool visible) = 0;
        virtual bool IsUiEnabled() const = 0;
        virtual void EnableVsync(bool flag) = 0;
        virtual bool IsVsyncEnabled() const = 0;
        virtual void Destroy() = 0;
        virtual void CaptureFrame(const String& filename) = 0;
        virtual void PauseRenderer(bool flag) = 0;
        virtual bool IsRendererPaused() const = 0;
        virtual void DrawUi(UiData* ui) = 0;
    };
}