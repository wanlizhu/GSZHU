#pragma once
#ifdef PLATFORM_WINDOWS

#include "Application/IApplication.h"
#include "Application/Windows/Monitor.h"
#include "Utilities/Delegate.h"

struct GLFWwindow;

namespace Wanli
{
    class DLLDECL WindowsApplication final : public IApplication
    {
    public:
        WindowsApplication();

        virtual ~WindowsApplication();
        virtual void Initialize(const WindowCreateInfo& info) override final;
        virtual void Update() override final;
        virtual void Shutdown() override final;
        virtual bool IsQuit() const override final;
        virtual WindowHandle GetWindowHandle() const override final;
        virtual glm::ivec2 GetContentSize() const override final;
        virtual glm::vec2 GetContentScale() const override final;
        virtual PCCHArray GetRequiredVulkanExtensions() const override final;
        virtual PCCHArray GetRequiredVulkanLayers() const override final;

        GLFWwindow* GetWindowGLFW() const;
        const Monitor& GetMonitor() const;
        glm::ivec2 GetPosition() const;

        void SetSize(const glm::ivec2& size);
        void SetPosition(const glm::ivec2& pos);
        void SetTite(const String& title);
        void SetIcons(const Array<Path>& paths);
        void SetIconified(bool iconify);
        void SetFloating(bool floating);
        void SetResizable(bool resizable);
        void SetBorderless(bool borderless);
        void SetFullscreen(bool fullscreen, Optional<Monitor> monitor = std::nullopt);

        inline const String& GetTitle() const { return mInfo.title; }
        inline bool IsFullscreen() const { return mInfo.fullscreen; }
        inline bool IsBorderless() const { return mInfo.borderless; }
        inline bool IsResizable() const { return mInfo.resizable; }
        inline bool IsFloating() const { return mInfo.floating; }
        inline bool IsFocused() const { return mFocused; }
        inline bool IsIconified() const { return mIconified; }

    protected:
        void CreateMenu();

    protected:
        WindowCreateInfo mInfo;
        GLFWwindow* mWindowGLFW = nullptr;
        Monitor mCurrentMonitor;

        bool mFocused = false;
        bool mIconified = false;
        glm::ivec2 mFullscreenSize = { 0, 0 };
    };
}
#endif