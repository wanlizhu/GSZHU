#pragma once

#include <GSZHU/IUserInput.h>
#include <GSZHU/Config.h>
#include <GSZHU/IRenderer.h>
#include <GSZHU/IEvent.h>

namespace GSZHU {
    class IWindow {
    public:
        static constexpr int NumBackBuffers = 3;

        virtual HANDLE GetHandle() const = 0;
        virtual STRING const& GetName() const = 0;
        virtual void Initialize() = 0;
        virtual void Destroy() = 0;
        virtual INT2 GetSize() const = 0;
        virtual INT2 GetPosition() const = 0;

        virtual bool IsValid() const = 0;
        virtual bool IsVSync() const = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsFullScreen() const = 0;
        virtual void SetFullScreen(bool enabled) = 0;
        
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void OnEvent(IEvent const& event) = 0;
        virtual void PostEvent(IEvent const& event) = 0;
        virtual void SetRenderer(std::weak_ptr<IRenderer> renderer) = 0;
        virtual void SetUserInputHandler(std::weak_ptr<IUserInput> handler) = 0;
    };
}