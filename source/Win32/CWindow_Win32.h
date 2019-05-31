#pragma once

#include <GSZHU/IWindow.h>
#include "Win32/System_Win32.h"
#include "CWindowClock.h"

namespace GSZHU {
    class CWindow_Win32 : public IWindow {
        friend LRESULT CALLBACK WinProc_Win32(HWND, UINT, WPARAM, LPARAM);
    public:
        CWindow_Win32(STRING const& name, int width, int height, bool isVSync = true);
        virtual ~CWindow_Win32();

        virtual HANDLE GetHandle() const override;
        virtual STRING const& GetName() const override;
        virtual void Initialize() override;
        virtual void Destroy() override;
        virtual INT2 GetSize() const override;
        virtual INT2 GetPosition() const override;

        virtual bool IsValid() const override;
        virtual bool IsVSync() const override;
        virtual void SetVSync(bool enabled) override;
        virtual bool IsFullScreen() const override;
        virtual void SetFullScreen(bool enabled) override;

        virtual void Show() override;
        virtual void Hide() override;
        virtual void OnEvent(IEvent const& event) override;
        virtual void PostEvent(IEvent const& event) override;
        virtual void SetRenderer(std::weak_ptr<IRenderer> renderer) override;
        virtual void SetUserInputHandler(std::weak_ptr<IUserInput> handler) override;

        INT2 const& GetLastMousePos() const;
        CWindowClock const& GetWindowClock() const;

    private:
        STRING mName;
        HWND mHandle = NULL;
        RECT mNormalRect = { 0, 0, 0, 0 };
        bool mVSync = false;
        bool mFullScreen = false;
        int mBackBufferIndex = 0;
        CWindowClock mWindowClock;
        INT2 mLastMousePos = { INT_MAX, INT_MAX };
        std::weak_ptr<IRenderer> mRenderer;
        std::weak_ptr<IUserInput> mUserInputHandler;
    };
}