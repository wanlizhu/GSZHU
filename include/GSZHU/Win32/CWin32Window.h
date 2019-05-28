#pragma once

#include <GSZHU/IWindow.h>
#include <GSZHU/Win32/System.h>

namespace GSZHU {
    class CWin32Window : public IWindow {
    public:
        CWin32Window(STRING const& Name, int Width, int Height, bool VSync = true);
        virtual ~CWin32Window();

        virtual void* GetNativeHandle() const override;
        virtual STRING const& GetName() const override;
        virtual void Initialize() override;
        virtual void Destroy() override;
        virtual INT2 GetSize() const override;
        virtual INT2 GetPosition() const override;

        virtual bool IsValid() const override;
        virtual bool IsVSync() const override;
        virtual void SetVSync(bool Enabled) override;
        virtual bool IsFullScreen() const override;
        virtual void SetFullScreen(bool Enabled) override;

        virtual void Show() override;
        virtual void Hide() override;
        virtual void SetUserInputHandler(std::weak_ptr<IUserInput> Handler) override;

        void OnWin32Message(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);

    private:
        HWND mWindow = NULL;
        RECT mNormalRect;
    };
}