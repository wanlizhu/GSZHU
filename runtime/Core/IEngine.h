#pragma once

#include "Core/Common.h"

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

    class DLL_DECL IEngine
    {
    public:
        virtual DrawContext* GetDrawContext() const = 0;
        virtual Framebuffer* GetFramebuffer() const = 0;
        virtual Window* GetWindow() const = 0;
        virtual Clock& GetClock() = 0;
        virtual bool IsKeyDown(const EKey& key) const = 0;
        virtual void ShowUi(bool visible) = 0;
        virtual bool IsUiEnabled() const = 0;
        virtual void Destroy() = 0;
        
    };
}