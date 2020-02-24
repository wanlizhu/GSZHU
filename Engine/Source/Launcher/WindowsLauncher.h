#pragma once

#include "Core/Engine.h"
#include "Launcher/Windows/Window.h"
#include "Core/Engine.h"

namespace Wanli
{
    class WindowsLauncher final : public Window
    {
    public:
        WindowsLauncher() = default;
        virtual ~WindowsLauncher();

        virtual void OnWindowCreated() override;
        virtual void OnWindowClose() override;
        virtual void OnWindowIconified() override;
        virtual void OnWindowEvent(WindowEvent::PTR) override;
        virtual void OnKeyboardEvent(KeyboardEvent::PTR) override;
        virtual void OnMouseEvent(MouseEvent::PTR) override;
        virtual void OnWindowMessage(MSG msg, WPARAM wParam, LPARAM lParam) override;

    private:
        Engine::PTR mEngine;
    };
}